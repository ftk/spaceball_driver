#ifndef VKEY_H
#define VKEY_H



int get_vkey(const char * str)
{
  if(!*str)
    return 0;

// CHECK_VKEY(VK_LBUTTON) -> if(stricmp(str, "LBUTTON") == 0) return VK_LBUTTON;
#define CHECK_VKEY(k) \
  if(stricmp(str, 3 + #k) == 0) return k;

  CHECK_VKEY(VK_LBUTTON)
    CHECK_VKEY(VK_RBUTTON)
    CHECK_VKEY(VK_CANCEL)
    CHECK_VKEY(VK_MBUTTON)
    CHECK_VKEY(VK_XBUTTON1)
    CHECK_VKEY(VK_XBUTTON2)
    CHECK_VKEY(VK_BACK)
    CHECK_VKEY(VK_TAB)
    CHECK_VKEY(VK_CLEAR)
    CHECK_VKEY(VK_RETURN)
    CHECK_VKEY(VK_SHIFT)
    CHECK_VKEY(VK_CONTROL)
    CHECK_VKEY(VK_MENU)
    CHECK_VKEY(VK_PAUSE)
    CHECK_VKEY(VK_CAPITAL)
    CHECK_VKEY(VK_KANA)
    //CHECK_VKEY(VK_HANGUEL)
    //CHECK_VKEY(VK_HANGUL)
    CHECK_VKEY(VK_JUNJA)
    CHECK_VKEY(VK_FINAL)
    CHECK_VKEY(VK_HANJA)
    CHECK_VKEY(VK_KANJI)
    CHECK_VKEY(VK_ESCAPE)
    CHECK_VKEY(VK_CONVERT)
    CHECK_VKEY(VK_NONCONVERT)
    CHECK_VKEY(VK_ACCEPT)
    CHECK_VKEY(VK_MODECHANGE)
    CHECK_VKEY(VK_SPACE)
    CHECK_VKEY(VK_PRIOR)
    CHECK_VKEY(VK_NEXT)
    CHECK_VKEY(VK_END)
    CHECK_VKEY(VK_HOME)
    CHECK_VKEY(VK_LEFT)
    CHECK_VKEY(VK_UP)
    CHECK_VKEY(VK_RIGHT)
    CHECK_VKEY(VK_DOWN)
    CHECK_VKEY(VK_SELECT)
    CHECK_VKEY(VK_PRINT)
    CHECK_VKEY(VK_EXECUTE)
    CHECK_VKEY(VK_SNAPSHOT)
    CHECK_VKEY(VK_INSERT)
    CHECK_VKEY(VK_DELETE)
    CHECK_VKEY(VK_HELP)
    CHECK_VKEY(VK_LWIN)
    CHECK_VKEY(VK_RWIN)
    CHECK_VKEY(VK_APPS)
    CHECK_VKEY(VK_SLEEP)
    CHECK_VKEY(VK_NUMPAD0)
    CHECK_VKEY(VK_NUMPAD1)
    CHECK_VKEY(VK_NUMPAD2)
    CHECK_VKEY(VK_NUMPAD3)
    CHECK_VKEY(VK_NUMPAD4)
    CHECK_VKEY(VK_NUMPAD5)
    CHECK_VKEY(VK_NUMPAD6)
    CHECK_VKEY(VK_NUMPAD7)
    CHECK_VKEY(VK_NUMPAD8)
    CHECK_VKEY(VK_NUMPAD9)
    CHECK_VKEY(VK_MULTIPLY)
    CHECK_VKEY(VK_ADD)
    CHECK_VKEY(VK_SEPARATOR)
    CHECK_VKEY(VK_SUBTRACT)
    CHECK_VKEY(VK_DECIMAL)
    CHECK_VKEY(VK_DIVIDE)
    CHECK_VKEY(VK_F1)
    CHECK_VKEY(VK_F2)
    CHECK_VKEY(VK_F3)
    CHECK_VKEY(VK_F4)
    CHECK_VKEY(VK_F5)
    CHECK_VKEY(VK_F6)
    CHECK_VKEY(VK_F7)
    CHECK_VKEY(VK_F8)
    CHECK_VKEY(VK_F9)
    CHECK_VKEY(VK_F10)
    CHECK_VKEY(VK_F11)
    CHECK_VKEY(VK_F12)
    CHECK_VKEY(VK_F13)
    CHECK_VKEY(VK_F14)
    CHECK_VKEY(VK_F15)
    CHECK_VKEY(VK_F16)
    CHECK_VKEY(VK_F17)
    CHECK_VKEY(VK_F18)
    CHECK_VKEY(VK_F19)
    CHECK_VKEY(VK_F20)
    CHECK_VKEY(VK_F21)
    CHECK_VKEY(VK_F22)
    CHECK_VKEY(VK_F23)
    CHECK_VKEY(VK_F24)
    CHECK_VKEY(VK_NUMLOCK)
    CHECK_VKEY(VK_SCROLL)
    CHECK_VKEY(VK_LSHIFT)
    CHECK_VKEY(VK_RSHIFT)
    CHECK_VKEY(VK_LCONTROL)
    CHECK_VKEY(VK_RCONTROL)
    CHECK_VKEY(VK_LMENU)
    CHECK_VKEY(VK_RMENU)
    CHECK_VKEY(VK_BROWSER_BACK)
    CHECK_VKEY(VK_BROWSER_FORWARD)
    CHECK_VKEY(VK_BROWSER_REFRESH)
    CHECK_VKEY(VK_BROWSER_STOP)
    CHECK_VKEY(VK_BROWSER_SEARCH)
    CHECK_VKEY(VK_BROWSER_FAVORITES)
    CHECK_VKEY(VK_BROWSER_HOME)
    CHECK_VKEY(VK_VOLUME_MUTE)
    CHECK_VKEY(VK_VOLUME_DOWN)
    CHECK_VKEY(VK_VOLUME_UP)
    CHECK_VKEY(VK_MEDIA_NEXT_TRACK)
    CHECK_VKEY(VK_MEDIA_PREV_TRACK)
    CHECK_VKEY(VK_MEDIA_STOP)
    CHECK_VKEY(VK_MEDIA_PLAY_PAUSE)
    CHECK_VKEY(VK_LAUNCH_MAIL)
    CHECK_VKEY(VK_LAUNCH_MEDIA_SELECT)
    CHECK_VKEY(VK_LAUNCH_APP1)
    CHECK_VKEY(VK_LAUNCH_APP2)
    CHECK_VKEY(VK_OEM_1)
    CHECK_VKEY(VK_OEM_PLUS)
    CHECK_VKEY(VK_OEM_COMMA)
    CHECK_VKEY(VK_OEM_MINUS)
    CHECK_VKEY(VK_OEM_PERIOD)
    CHECK_VKEY(VK_OEM_2)
    CHECK_VKEY(VK_OEM_3)
    CHECK_VKEY(VK_OEM_4)
    CHECK_VKEY(VK_OEM_5)
    CHECK_VKEY(VK_OEM_6)
    CHECK_VKEY(VK_OEM_7)
    CHECK_VKEY(VK_OEM_8)
    CHECK_VKEY(VK_OEM_102)
    CHECK_VKEY(VK_PROCESSKEY)
    CHECK_VKEY(VK_PACKET)
    CHECK_VKEY(VK_ATTN)
    CHECK_VKEY(VK_CRSEL)
    CHECK_VKEY(VK_EXSEL)
    CHECK_VKEY(VK_EREOF)
    CHECK_VKEY(VK_PLAY)
    CHECK_VKEY(VK_ZOOM)
    CHECK_VKEY(VK_NONAME)
    CHECK_VKEY(VK_PA1)
    CHECK_VKEY(VK_OEM_CLEAR)

    if(str[1] == '\0')
    {
      if(*str >= '0' && *str <= '9')
        return *str - '0' + 0x30;
      if(*str >= 'A' && *str <= 'Z')
        return *str - 'A' + 0x41;

    }
    int key;
  if(sscanf(str, "%x", &key) == 1)
    return key;
  //if(sscanf(str, "%d", &key) == 1)
  //return key;

    return 0;
}


void sendinput_kb(int vkey, bool down)
{
  INPUT ip;

  if(vkey >= VK_LBUTTON && vkey <= VK_XBUTTON2 && vkey != VK_CANCEL) // mouse buttons
  {
      ip.type = INPUT_MOUSE;
      ip.mi.dx = ip.mi.dy = 0;
      ip.mi.mouseData = 0;
      ip.mi.dwFlags = 0;
      ip.mi.time = 0;
      ip.mi.dwExtraInfo = 0;
      switch(vkey)
      {
      case VK_XBUTTON1:
          ip.mi.dwFlags = MOUSEEVENTF_XDOWN << (!down);
          ip.mi.mouseData = XBUTTON1;
          break;
      case VK_XBUTTON2:
          ip.mi.dwFlags = MOUSEEVENTF_XDOWN << (!down);
          ip.mi.mouseData = XBUTTON2;
          break;
      case VK_LBUTTON:
          ip.mi.dwFlags = MOUSEEVENTF_LEFTDOWN << (!down);
          break;
      case VK_RBUTTON:
          ip.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN << (!down);
          break;
      case VK_MBUTTON:
          ip.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN << (!down);
          break;
      }
  }
  else
  {
      // Set up a generic keyboard event.
      ip.type = INPUT_KEYBOARD;
      ip.ki.wScan = 0;
      //ip.ki.wScan = MapVirtualKey(vkey, MAPVK_VK_TO_VSC);
      ip.ki.time = 0;
      ip.ki.dwExtraInfo = 0;

      ip.ki.wVk = vkey;
      ip.ki.dwFlags = 0; // 0 for key press
      if(!down)
          ip.ki.dwFlags |= KEYEVENTF_KEYUP;
  }
  SendInput(1, &ip, sizeof(INPUT));
}

void sendinput_mouse(unsigned int type, int32_t m)
{
    INPUT ip;

    ip.type = INPUT_MOUSE;
    ip.mi.dx = ip.mi.dy = 0;
    ip.mi.mouseData = 0;
    ip.mi.dwFlags = 0;
    ip.mi.time = 0;
    ip.mi.dwExtraInfo = 0;


    switch(type)
    {
    case 1:
        ip.mi.dwFlags |= MOUSEEVENTF_MOVE;
        ip.mi.dx = m;
        break;
    case 2:
        ip.mi.dwFlags |= MOUSEEVENTF_MOVE;
        ip.mi.dy = m;
        break;
    case 3:
        ip.mi.dwFlags |= MOUSEEVENTF_HWHEEL;
        ip.mi.mouseData = m;
        break;
    case 4:
        ip.mi.dwFlags |= MOUSEEVENTF_WHEEL;
        ip.mi.mouseData = m;
        break;

    }

    SendInput(1, &ip, sizeof(INPUT));
}

#endif /* VKEY_H */
