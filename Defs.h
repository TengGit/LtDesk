#ifndef _0T_DEFS_H_INCLUDED
#define _0T_DEFS_H_INCLUDED

/* Copyright (C) 2016 Teng K. J.
   All rights reserved.
*/

#include <windows.h>
#include <tchar.h>

namespace tl {

extern class App {
public:
    friend int APIENTRY ::WinMain(HINSTANCE, HINSTANCE, LPSTR, int);

    operator HINSTANCE() {return hThis;}
    HINSTANCE GetPrevInstance() {return hPrev;}
    int RunApplication();
private:
    HINSTANCE hThis, hPrev;
} Application;

}

/* This is the user start function. Do not use WinMain. */
extern int APIENTRY Main(int nCmdShow);

#endif // _0T_DEFS_H_INCLUDED
