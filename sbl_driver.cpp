#include <cstdint>
#define WINVER 0x610
#define _WIN32_WINNT WINVER
#include <windows.h>

#include <cstdio>
#include <cassert>

#include "vjoy.h"

#include "vkey.h"


#include "sball.h"



int main(int argc, const char * argv[])
{

  UINT js_id = 1;

  char comport[64] = "COM1";

  char initstr[256];
  unsigned int initlen = 0;

  // input - column, output - row
  float sensmatrix[6][6] = {0};
  for(int i = 0; i < 6; i++)
    sensmatrix[i][i] = 25;

  int offset[6] = {0};

  int keybinds[12] = {0};
  int mousebinds[6] = {0};

  FILE * fp = fopen(argc > 1 ? argv[1] : "settings.txt", "r");
  if(fp)
  {
    fscanf(fp, "%d", &js_id); // js number (1)
    fscanf(fp, "%s", comport); // com port (COM1)

    fscanf(fp, "%u", &initlen);
    for(int i = 0; i < initlen; i++)
      fscanf(fp, "%hhx", &initstr[i]);

    // sensitivity settings
    for(int i = 0; i < 6; i++)
      for(int j = 0; j < 6; j++)
        fscanf(fp, "%f", &sensmatrix[i][j]);

    // offset
    for(int i = 0; i < 6; i++)
      fscanf(fp, "%d", &offset[i]);

    for(int i = 0; i < 12; i++)
    {
      char tmp[64];
      fscanf(fp, "%s", tmp);
      keybinds[i] = get_vkey(tmp);
      if(keybinds[i])
        printf("Button %d bound to 0x%x\n", i+1, keybinds[i]);
    }

    for(int i = 0; i < 6; i++)
      fscanf(fp, "%d", &mousebinds[i]);

    fclose(fp);
  }
  else
    printf("No settings file!\n");


  if(js_id)
  {
    vjoy_setup(js_id);
  }

  fflush(stdout);

  sbl::setup(comport);

  if(initlen)
    sbl::write(initstr, initlen);

  sbl::handle h;

  auto n = [&sensmatrix,&offset] (float * col, int idx) -> int {
    auto clamp = [] (int val, int min, int max) { return val < min ? min : (val > max ? max : val); };
    // from the spaceball coords to 1 - 8000
    float res = 0;
    for(int i = 0; i < 6; i++)
      res += col[i] * sensmatrix[idx][i];
    return clamp(res + offset[idx], 1-0x4000, 0x4000);
  };

  while(1)
  {
    sbl::update(&h);


    float col[6] = { h.trans[0],h.trans[1],h.trans[2],h.rot[0],h.rot[1],h.rot[2]};

    float row[6];
    for(int i = 0; i < 6; i++) row[i] = n(col, i);

    if(js_id)
    {
      if(!vjoy_update(js_id, row, h.buttons & ((1 << 12) - 1)))
      {
        printf("error! cant update vjd %d\n", js_id);
        js_id = 0;
      }
    }


    for(unsigned int i = 0; i < 6; i++)
      if(mousebinds[i])
        sendinput_mouse(mousebinds[i], row[i]);

    static int oldbuttons = 0;
    for(unsigned int i = 0; i < 12; i++)
    {
      if(keybinds[i])
      {
        if(((h.buttons >> i)&1) != ((oldbuttons >> i)&1))
        {
          sendinput_kb(keybinds[i], (h.buttons >> i)&1);
#ifdef DEBUG
          printf("sent kb event 0x%02X %s\n", keybinds[i], ((h.buttons >> i)&1) ? "DOWN" : "UP");
          fflush(stdout);
#endif
        }
      }
    }
    oldbuttons = h.buttons;


    //printf("update!\n");
    //fflush(stdout);
  }


}


