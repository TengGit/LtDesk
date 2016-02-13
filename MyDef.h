#ifndef MYDEF_H_INCLUDED
#define MYDEF_H_INCLUDED

/* Copyright (C) 2016 Teng K. J.
   All rights reserved.
*/

#include "tgui.h"
#include "ExtWinDef.h"
#include "resource.h"

#define MOUSE_STATE_MASK (MK_LBUTTON | MK_RBUTTON | MK_MBUTTON)

#ifndef SPI_GETDESKWALLPAPER
#define SPI_GETDESKWALLPAPER 0x0073
#endif // SPI_GETDESKWALLPAPER
#ifndef WS_EX_NOACTIVATE
#define WS_EX_NOACTIVATE 0x8000000 /* w2k */
#endif // WS_EX_NOACTIVE

#include <shlobj.h>

extern TCHAR szHello[];
extern TCHAR helloWorld[];
extern TCHAR szButtonExitName[];
#define WND_NAME _T("Light Desktop")

int Pause(int millis, bool bDoEvents);
int GetRelativeRect(HWND hwnd, LPRECT rect);
void DoEvents();

enum {
    IDM_SHOWHELLO = 101,
    IDM_EXIT,
};
MenuItem items[];

#endif // MYDEF_H_INCLUDED
