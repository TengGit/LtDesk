#ifndef MYDEF_H_INCLUDED
#define MYDEF_H_INCLUDED

/* Copyright (C) 2016 Teng K. J.
   All rights reserved.
*/

#if !__GNUC__
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

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
#ifndef HIMETRIC_PER_INCH
#define HIMETRIC_PER_INCH 2540
#endif // HIMETRIC_PER_INCH

//#include <shlobj.h>

#define REG_PROG_ROOT HKEY_CURRENT_USER
extern TCHAR regProgRoot[];
extern TCHAR regBootValueName[];

extern TCHAR helloWorld[];
extern TCHAR szButtonExitName[];
extern TCHAR msgRegSetFail[];
extern TCHAR msgInstance[];
#define WND_NAME _T("Light Desktop")

int Pause(int millis, bool bDoEvents);
int GetRelativeRect(HWND hwnd, LPRECT rect);
void DoEvents();
bool PointInSP(const POINT&, const tl::SizeAndPos&);
HBITMAP IconToBitmap(HICON);

bool ValueInRegistry(HKEY hKey, LPCTSTR valueName);

enum {
    IDM_RUNONBOOT = 101,
    IDM_EXIT,
    IDM_LOADING,
    DYN_MENU_BEGIN
};
extern tl::MenuItem items[];
extern tl::MenuItem loading[];
extern tl::MenuItem nofile[];

extern TCHAR desktopPath[];

#endif // MYDEF_H_INCLUDED
