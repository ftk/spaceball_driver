#ifndef SBALL_H
#define SBALL_H

namespace sbl {

static HANDLE port;

void clear_error()
{
  DWORD errors;
  COMSTAT stat;
  ClearCommError(port, &errors, &stat);
  if(errors)
    printf("commerror: %x\n", errors);
}

int read(char * buf, int size)
{
  DWORD num = size;
  int ok = ReadFile(port, buf, size, &num, NULL);
  //assert(ok);
  if(!ok)
  {
    clear_error();
    return 0;
  }
  return num;
}

int write(const char * buf, int size)
{
  DWORD num = size;
  int ok = WriteFile(port, buf, size, &num, NULL);
  //assert(ok);
  if(!ok)
  {
    clear_error();
    return write(buf, size);
  }
  return num;
}

struct handle {
  unsigned char buf[256] = {0};
  unsigned int bufpos=0;        /* current char position in packet buffer */
  int packtype=0;      /* what kind of packet is it */
  unsigned int packlen=1;       /* how many bytes do we ultimately expect? */
  int escapedchar=0;   /* if set, we're processing an escape sequence */
  int erroroccured=0;  /* if set, we've received an error packet or packets */
  int resetoccured=0;  /* if set, ball was reset, so have to reinitialize it */
  int spaceball4000=0; /* if set, its a Spaceball 4000 */
  int leftymode4000=0; /* if set, Spaceball 4000 in "lefty" orientation */
  int trans[3]={0};      /* last translational data received */
  int rot[3]={0};        /* last rotational data received */
  unsigned int buttons=0;       /* current button status */
  unsigned int timer=0;         /* time since last packet was received */
};


void setup(const char * comport, int update = 100)
{
  port = CreateFile(comport, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0,NULL);

  assert(port != INVALID_HANDLE_VALUE);

  if(!::SetupComm(port, 16, 16))
  {
    printf("Failed to SetupComm (%d)", GetLastError());
    exit(1);
  }

  DCB dcb;
  memset(&dcb, 0, sizeof(dcb));
  dcb.DCBlength = sizeof(dcb);

  /*if(!::GetCommState(port, &dcb))
  {
    printf("Failed to Get Comm State (%d)", GetLastError());
    exit(1);
    }*/

  dcb.BaudRate  = CBR_9600;
  dcb.ByteSize  = 8;
  dcb.Parity    = NOPARITY;
  dcb.fBinary   = 1;
  dcb.StopBits  = ONESTOPBIT;
  dcb.fInX = 1;
  dcb.fOutX = 1;
  dcb.XonChar = 0x11;
  dcb.XoffChar = 0x13;
  dcb.XonLim = 16;
  dcb.XoffLim = 16;
  dcb.fDtrControl = DTR_CONTROL_HANDSHAKE;
  dcb.fOutxCtsFlow = 1;
  dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
  dcb.fOutxDsrFlow = 1;
  dcb.fDsrSensitivity = 1;
  dcb.EofChar = '\r';
  dcb.fAbortOnError = 1;


  if (!::SetCommState(port, &dcb))
  {
    printf("Failed to Set Comm State (%d)\n", GetLastError());
    exit(1);
  }

  COMMTIMEOUTS timeout;
  // read will return as soon as possible
  timeout.ReadIntervalTimeout = MAXDWORD;
  timeout.ReadTotalTimeoutMultiplier = MAXDWORD;
  timeout.ReadTotalTimeoutConstant = update; // wait at most (update) ms for data, then return

  timeout.WriteTotalTimeoutMultiplier = 0;
  timeout.WriteTotalTimeoutConstant = 0;


  if(!::SetCommTimeouts(port, &timeout))
  {
    printf("Failed to SetCommTimeouts (%d)\n", GetLastError());
    exit(1);
  }

  Sleep(1500);

  PurgeComm(port, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
  clear_error();


  //write("M\r", 2); //set mode
  //write("YC\r", 3); //?
  //write("AD\r", 3); //?

}

void close()
{
  CloseHandle(port);
}

unsigned int update(handle * handle)
{
  unsigned int i, j, num, packs = 0;
  char rawbuf[32];

  num = read(rawbuf, sizeof(rawbuf));

  if (num > 0) {
    for (i=0; i<num; i++) {


      /* process potentially occuring escaped character sequences */
      if (rawbuf[i] == '^') {
        if (!handle->escapedchar) {
          handle->escapedchar = 1;
          continue; /* eat the escape character from buffer */
        }
      }

      if (handle->escapedchar) {
        handle->escapedchar = 0;

        switch (rawbuf[i]) {
        case '^': /* leave char in buffer unchanged */
          break;

        case 'Q':
        case 'S':
        case 'M':
          rawbuf[i] &= 0x1F; /* convert character to unescaped form */
          break;

        default:
#if defined(DEBUG)
          printf("\nGot a bad escape sequence! 0x%02x", rawbuf[i]);
          if (isprint(rawbuf[i]))
            printf(" (%c)", rawbuf[i]);
          else
            printf(" (unprintable)");
          printf("\n");
#endif
          // ignore escape, push ^ before current symbol
          handle->buf[handle->bufpos++] = '^';
          break;
        }
      }


      /* figure out what kind of packet we received */
      if (handle->bufpos == 0) {
        switch(rawbuf[i]) {
        case 'D':  /* Displacement packet */
          handle->packtype = 'D';
          handle->packlen = 16;    /* D packets are 15 bytes long */
          break;

        case 'K':  /* Button/Key packet */
          handle->packtype = 'K';
          handle->packlen = 4;     /* K packets are 3 bytes long */
          break;

        case '.': /* Spaceball 4000 FLX "advanced" button press event */
          handle->packtype = '.';
          handle->packlen = 4;     /* . packets are 3 bytes long */
          break;

        case 'C': /* Communications mode packet */
          handle->packtype = 'C';
          handle->packlen = 4;
          break;

        case 'F': /* Spaceball sensitization mode packet */
          handle->packtype = 'F';
          handle->packlen = 4;
          break;

        case 'M': /* Movement mode packet */
          handle->packtype = 'M';
          handle->packlen = 5;
          break;

        case 'N': /* Null region packet */
          handle->packtype = 'N';
          handle->packlen = 3;
          break;

        case 'P': /* Update rate packet */
          handle->packtype = 'P';
          handle->packlen = 6;
          break;

        case '\v': /* XON at poweron */
          handle->packtype = '\v';
          handle->packlen = 1;
          break;

        case '\n': /* carriage return at poweron */
        case '\r': /* carriage return at poweron */
          handle->packtype = '\r';
          handle->packlen = 1;
          break;

        case '@': /* Spaceball Hard/Soft Reset packet */
          handle->resetoccured=1;
          handle->packtype = '@';
          handle->packlen = 62;    /* Resets aren't longer than 62 chars */
          break;

        case 'E': /* Error packet */
          handle->packtype = 'E';
          handle->packlen = 8;     /* E packets are up to 7 bytes long */
          break;

        case 'Z': /* Zero packet (Spaceball 2003/3003/4000 FLX) */
          handle->packtype = 'Z';
          handle->packlen = 14;    /* Z packets are hardware dependent */
          break;

        default:  /* Unknown packet! */
#if defined(DEBUG)
          printf("\nUnknown packet (1): 0x%02x \n ", rawbuf[i]);
          printf("                char:  ");
          if (isprint(rawbuf[i]))
            printf("%c", rawbuf[i]);
          else
            printf(" (unprintable)");
          printf("\n");
#endif
          continue;
        }
      }


      handle->buf[handle->bufpos] = rawbuf[i];
      handle->bufpos++;

      /* Reset packet processing */
      if (handle->packtype == '@') {
        if (rawbuf[i] != '\r')
          continue;
        else
          handle->packlen = handle->bufpos;
      }

      /* Error packet processing */
      if (handle->packtype == 'E') {
        if (rawbuf[i] != '\r')
          continue;
        else
          handle->packlen = handle->bufpos;
      }
      else if (handle->bufpos != handle->packlen)
        continue;

#if defined(DEBUG)
      printf("%c", handle->packtype);
      for(int i = 0; i < handle->packlen - 1; i++)
        printf("%02x", handle->buf[1 + i]);
      printf("\n");
#endif
      switch (handle->packtype)
      {
      case 'D':  /* ball displacement event */
      {
        unsigned int tx, ty, tz, rx, ry, rz;

        /* number of 1/16ths of milliseconds since last */
        /* ball displacement packet */
        handle->timer = ((handle->buf[1]) << 8) | (handle->buf[2]);

        tx = ((handle->buf[ 3]) << 8) | ((handle->buf[ 4]));
        ty = ((handle->buf[ 5]) << 8) | ((handle->buf[ 6]));
        tz = ((handle->buf[ 7]) << 8) | ((handle->buf[ 8]));
        rx = ((handle->buf[ 9]) << 8) | ((handle->buf[10]));
        ry = ((handle->buf[11]) << 8) | ((handle->buf[12]));
        rz = ((handle->buf[13]) << 8) | ((handle->buf[14]));

        handle->trans[0] = (((int32_t) tx) << 16) >> 16;
        handle->trans[1] = (((int32_t) ty) << 16) >> 16;
        handle->trans[2] = (((int32_t) tz) << 16) >> 16;
        handle->rot[0]   = (((int32_t) rx) << 16) >> 16;
        handle->rot[1]   = (((int32_t) ry) << 16) >> 16;
        handle->rot[2]   = (((int32_t) rz) << 16) >> 16;
      }
      break;

      case 'K': /* button press event */
        /* Spaceball 2003A, 2003B, 2003 FLX, 3003 FLX, 4000 FLX       */
        /* button packet. (4000 only for backwards compatibility)     */
        /* The lowest 5 bits of the first byte are buttons 5-9        */
        /* Button '8' on a Spaceball 2003 is the rezero button        */
        /* The lowest 4 bits of the second byte are buttons 1-4       */
        /* For Spaceball 2003, we'll map the buttons 1-7 normally     */
        /* skip 8, as its a hardware "rezero button" on that device   */
        /* and call the "pick" button "8".                            */
        /* On the Spaceball 3003, the "right" button also triggers    */
        /* the "pick" bit.  We OR the 2003/3003 rezero bits together  */

        /* if we have found a Spaceball 4000, then we ignore the 'K'  */
        /* packets entirely, and only use the '.' packets.            */
        if (handle->spaceball4000)
          break;

        handle->buttons =
          ((handle->buf[1] & 0x10) <<  3) | /* 2003 pick button is "8" */
          ((handle->buf[1] & 0x20) <<  9) | /* 3003 rezero button      */
          ((handle->buf[1] & 0x08) << 11) | /* 2003 rezero button      */
          ((handle->buf[1] & 0x07) <<  4) | /* 5,6,7    (2003/4000)    */
          ((handle->buf[2] & 0x30) <<  8) | /* 3003 Left/Right buttons */
          ((handle->buf[2] & 0x0F));        /* 1,2,3,4  (2003/4000)    */
        break;

      case '.': /* button press event (4000) */
        /* Spaceball 4000 FLX "expanded" button packet, with 12 buttons */

        /* extra packet validity check, since we use this packet type */
        /* to override the 'K' button packets, and determine if its a */
        /* Spaceball 4000 or not...                                   */
        if (handle->buf[3] != '\r') {
          break; /* if not terminated with a '\r', probably garbage */
        }

        /* if we got a valid '.' packet, this must be a Spaceball 4000 */
#if defined(DEBUG)
        if (!handle->spaceball4000)
          printf("\nDetected a Spaceball 4000 FLX\n");
#endif
        handle->spaceball4000 = 1; /* Must be talking to a Spaceball 4000 */

        /* Spaceball 4000 series "expanded" button press event      */
        /* includes data for 12 buttons, and left/right orientation */
        handle->buttons =
          (((~handle->buf[1]) & 0x20) << 10) |  /* "left handed" mode  */
          ((handle->buf[1] & 0x1F) <<  7)    |  /* 8,9,10,11,12        */
          ((handle->buf[2] & 0x3F)      )    |  /* 1,2,3,4,5,6 (4000)  */
          ((handle->buf[2] & 0x80) >>  1);      /* 7           (4000)  */

#if defined(DEBUG)
        if (handle->leftymode4000 != ((handle->buf[1] & 0x20) == 0))
          printf("\nSpaceball 4000 mode changed to: %s\n",
                 (((handle->buf[1] & 0x20) == 0) ? "left handed" : "right handed"));
#endif
        /* set "lefty" orientation mode if "lefty bit" is _clear_ */
        if ((handle->buf[1] & 0x20) == 0)
          handle->leftymode4000 = 1; /* left handed mode */
        else
          handle->leftymode4000 = 0; /* right handed mode */
        break;

      case 'C': /* Communications mode packet */
      case 'F': /* Spaceball sensitization packet */
      case 'P': /* Spaceball update rate packet */
      case 'M': /* Spaceball movement mode packet */
      case 'N': /* Null region packet */
      case '\r': /* carriage return at poweron */
      case '\v': /* XON at poweron */
        /* eat and ignore these packets */
        break;

      case '@': /* Reset packet */
#ifdef DEBUG
        printf("Spaceball reset: ");
        for (j=0; j<handle->packlen; j++) {
          if (isprint(handle->buf[j]))
            printf("%c", handle->buf[j]);
        }
        printf("\n");
#endif
        /* if we get a reset packet, we have to re-initialize       */
        /* the device, and assume that its completely schizophrenic */
        /* at this moment, we must reset it again at this point     */
        handle->resetoccured=1;
        //write("@\r", 2);
        break;


      case 'E': /* Error packet, hardware/software problem */
        handle->erroroccured++;
#ifdef DEBUG
        printf("\nSpaceball Error!!    ");
        printf("Error code: ");
        for (j=0; j<handle->packlen; j++) {
          printf(" 0x%02x ", handle->buf[j]);
        }
        printf("\n");
#endif
        break;

      case 'Z': /* Zero packet (Spaceball 2003/3003/4000 FLX) */
        /* We just ignore these... */
        break;

      default:
#ifdef DEBUG
        printf("Unknown packet (2): 0x%02x\n", handle->packtype);
        printf("              char:  ");
        if (isprint(handle->packtype))
          printf("%c", handle->packtype);
        else
          printf(" (unprintable)");
        printf("\n");
#endif
        break;
      }

      /* reset */
      handle->bufpos = 0;
      handle->packtype = 0;
      handle->packlen = 1;
      packs++;
    }
  }

  return packs;
}

} // namespace sbl

#endif /* SBALL_H */
