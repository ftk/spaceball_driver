#ifndef VJOY_H
#define VJOY_H

#ifndef NO_VJOY
#include "public.h"
#include "vjoyinterface.h"
#endif

void vjoy_setup(unsigned js_id)
{
#ifndef NO_VJOY
  if (!vJoyEnabled())
  {
    printf("Failed Getting vJoy attributes.\n");
    exit(1);
  }
  else
  {
    printf("Vendor: %S\nProduct :%S\nVersion Number:%S\n",\
           TEXT(GetvJoyManufacturerString()),\
           TEXT(GetvJoyProductString()),\
           TEXT(GetvJoySerialNumberString()));
  };

  WORD VerDll, VerDrv;
  if (!DriverMatch(&VerDll, &VerDrv))
    printf("Failed\r\nvJoy Driver (version %04x) does not match vJoyInterface DLL(version %04x)\n", VerDrv ,VerDll);
  else
    printf( "OK - vJoy Driver and vJoyInterface DLL match vJoyInterface DLL (version%04x)\n", VerDrv);
  // Get the state of the requested device
  VjdStat status = GetVJDStatus(js_id);
  switch (status)
  {
  case VJD_STAT_OWN:
    printf("vJoy Device %d is already owned by this feeder\n", js_id);
    break;
  case VJD_STAT_FREE:
    printf("vJoy Device %d is free\n", js_id);
    break;
  case VJD_STAT_BUSY:
    printf("vJoy Device %d is already owned by another feeder\nCannot continue\n",
           js_id);
    exit(1);
  case VJD_STAT_MISS:
    printf("vJoy Device %d is not installed or disabled\nCannot continue\n",
           js_id);
    exit(1);
  default:
    printf("vJoy Device %d general error\nCannot continue\n", js_id);
    exit(1);
  };


  // Acquire the target
  if ((status == VJD_STAT_OWN) || ((status == VJD_STAT_FREE) && (!
                                                                 AcquireVJD(js_id))))
  {
    printf("Failed to acquire vJoy device number %d.\n", js_id);
    exit(1);
  }
  else
  {
    printf("Acquired: vJoy device number %d.\n", js_id);
  }

/*
// Check which axes are supported
BOOL AxisX = GetVJDAxisExist(js_id, HID_USAGE_X);
BOOL AxisY = GetVJDAxisExist(js_id, HID_USAGE_Y);
BOOL AxisZ = GetVJDAxisExist(js_id, HID_USAGE_Z);
BOOL AxisRX = GetVJDAxisExist(js_id, HID_USAGE_RX);
// Get the number of buttons supported by this vJoy device
int nButtons = GetVJDButtonNumber(js_id);
// Print results
printf("\nvJoy Device %d capabilities\n", js_id);
printf("Numner of buttons\t\t%d\n", nButtons);
printf("Axis X\t\t%s\n", AxisX?"Yes":"No");
printf("Axis Y\t\t%s\n", AxisX?"Yes":"No");
printf("Axis Z\t\t%s\n", AxisX?"Yes":"No");
printf("Axis Rx\t\t%s\n", AxisRX?"Yes":"No");
*/

  // Reset this device to default values
  ResetVJD(js_id);
#endif
}


#ifndef NO_VJOY
static JOYSTICK_POSITION_V2 js;

bool vjoy_update(unsigned js_id, const float row[6], unsigned buttons)
{
  js.bDevice = js_id;

  js.wAxisX = row[0] + 0x4000;
  js.wAxisY = row[1] + 0x4000;
  js.wAxisZ = row[2] + 0x4000;
  js.wAxisXRot = row[3] + 0x4000;
  js.wAxisYRot = row[4] + 0x4000;
  js.wAxisZRot = row[5] + 0x4000;

  js.lButtons = buttons;


  return UpdateVJD(js_id, &js);
}

#else
bool vjoy_update(unsigned js_id, const float row[6], unsigned buttons)
{
  printf("VJoy support was disabled\n");
  return false;
}

#endif

void vjoy_close(unsigned js_id)
{
#ifndef NO_VJOY
  RelinquishVJD(js_id);
#endif
}

#endif /* VJOY_H */
