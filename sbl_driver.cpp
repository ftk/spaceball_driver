#include <cstdint>
#define WINVER 0x610
#define _WIN32_WINNT WINVER
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <cstdio>
#include <cassert>
#include <ctime>
#include <cmath>

#include "vjoy.h"

#include "vkey.h"


#include "sball.h"


template<typename T>
T clamp(T val, T min, T max) { return val < min ? min : (val > max ? max : val); };


uint64_t get_counter()
{
  uint64_t c;
  QueryPerformanceCounter((LARGE_INTEGER*)&c);
  return c;
}

int main(int argc, const char * argv[])
{

  UINT js_id = 1;

  char comport[64] = "COM1";

 // input - column, output - row
  float sensmatrix[6][6] = {0};
  for(int i = 0; i < 6; i++)
    sensmatrix[i][i] = 1;

  int min[6] = {0}, max[6] = {0};
  float power[6] = {1,1,1,1,1,1};
  float offset[6] = {0};

  int keybinds[12] = {0};
  int mousebinds[6] = {0};
  float mousesens[6] = {0};

  int arg = 1;
  if(stricmp(argv[arg], "--hide") == 0)
  {
    FreeConsole();
    arg++;
  }

  FILE * fp = fopen(argc > 1 ? argv[arg] : "settings.txt", "r");
  if(fp)
  {
    fscanf(fp, "%d", &js_id); // js number (1)
    fscanf(fp, "%s", comport); // com port (COM1)

    fscanf(fp, "%u", &sbl::initlen);
    for(unsigned int i = 0; i < sbl::initlen; i++)
      fscanf(fp, "%hhx", &sbl::initstr[i]);


    // min, max, power
    for(int i = 0; i < 6; i++)
      fscanf(fp, "%d %d %f", &min[i], &max[i], &power[i]);


    // sensitivity settings
    for(int i = 0; i < 6; i++)
      for(int j = 0; j < 6; j++)
        fscanf(fp, "%f", &sensmatrix[i][j]);

    // offset
    for(int i = 0; i < 6; i++)
      fscanf(fp, "%f", &offset[i]);

    for(int i = 0; i < 12; i++)
    {
      char tmp[64] = {0};
      fscanf(fp, "%s", tmp);
      keybinds[i] = get_vkey(tmp);
      if(keybinds[i])
        printf("Button %d bound to 0x%x\n", i+1, keybinds[i]);
    }

    for(int i = 0; i < 6; i++)
      fscanf(fp, "%d %f", &mousebinds[i], &mousesens[i]);

    fclose(fp);
  }
  else
    printf("No settings file!\n");


  if(js_id)
  {
    vjoy_setup(js_id);
  }

  sbl::setup(comport, 10);
  atexit(sbl::close);

  sbl::handle h;

  uint64_t perf_freq;
  QueryPerformanceFrequency((LARGE_INTEGER*)&perf_freq);

  float row[6] = {0};

  uint64_t last_update = 0;

  while(1)
  {
    int events = sbl::update(&h);

    if(events)
    {
      // update row
      float col[6] = { h.trans[0],h.trans[1],h.trans[2],h.rot[0],h.rot[1],h.rot[2]};

      // normalize -> [-1, 1]
      for(int i = 0; i < 6; i++)
      {
        col[i] = (col[i] < 0) ? (col[i] / -min[i]) : (col[i] / max[i]);
        if(power[i] != 1.f)
        {
          if(col[i] < 0)
            col[i] = -pow(-col[i], power[i]);
          else
            col[i] = pow(col[i], power[i]);
        }
      }


      for(int i = 0; i < 6; i++)
      {
        row[i] = 0;
        for(int j = 0; j < 6; j++)
          row[i] += col[j] * sensmatrix[i][j];
        row[i] += offset[i];
      }


      if(js_id)
      {
        if(!vjoy_update(js_id, row, h.buttons & ((1 << 12) - 1)))
        {
          printf("error! cant update vjd %d\n", js_id);
          js_id = 0;
        }
      }


      static unsigned int oldbuttons = 0;
      if(h.buttons != oldbuttons)
      {
        if((h.buttons & ((1 << 12) - 1)) == 0x0f) // 0x0f == 0b1111 - first 4 buttons are pressed
        {
          printf("Buttons 1+2+3+4 pressed, exiting.\n");
          if(js_id)
            vjoy_close(js_id);
          return 0;
        }
        for(unsigned int i = 0; i < 12; i++)
        {
          if(keybinds[i])
          {
            if(((h.buttons >> i)&1) != ((oldbuttons >> i)&1))
            {
              sendinput_kb(keybinds[i], (h.buttons >> i)&1);
#ifdef DEBUG
              printf("sent kb event 0x%02X %s\n", keybinds[i], ((h.buttons >> i)&1) ? "DOWN" : "UP");
#endif
            }
          }
        }
        oldbuttons = h.buttons;
      }

      last_update = get_counter();
    }

    static unsigned mouseupds = 0;
    const int interp = 1;
    if(get_counter() >= last_update || events)
    {
      for(unsigned int i = 0; i < 6; i++)
        if(mousebinds[i])
        {
          int mov = std::round(row[i] * mousesens[i]);
          if(!mov)
            continue;
          //  interpolate mouse X and Y movement
          // send interp times more mouse events
          //if(mousebinds[i] == 1 || mousebinds[i] == 2)
          if(0)
          {
            sendinput_mouse(mousebinds[i], mov / interp + (events ? (mov % interp) : 0));
          }
          else if(events) // just send once
            sendinput_mouse(mousebinds[i], mov);
        }
      // update mouse at 20Hz*inerp = 80Hz
      last_update += (h.timer * perf_freq / 16000) / interp;
      mouseupds++;
    }

    #ifdef DEBUG
    {
      static int64_t t = 0;
      static unsigned evupdates, updates;
      if(t <= time(nullptr))
      {
        fprintf(stderr, "device %llu total %llu mouse %llu Hz\n", evupdates, updates, mouseupds);
        updates = evupdates = mouseupds = 0;
        t = time(nullptr) + 1;
      }

      if(events)
        evupdates++;
      updates++;
    }
    #endif


//#ifdef DEBUG

//if(events)
//fprintf(stderr, "%5d %5d %5d %5d %5d %5d\r", h.trans[0],h.trans[1],h.trans[2],h.rot[0],h.rot[1],h.rot[2]);


//#endif
  }


}


