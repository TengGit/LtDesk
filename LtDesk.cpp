/* Copyright (C) 2016 Teng K. J.
   All rights reserved.
*/

#include "tgui.h"
#include "MyWindow.h"

using namespace tl;

//static void ProcOnResize(HWND);

int APIENTRY Main(int nCmdShow) {
    MyWindow wnd;
    wnd.Create();
    return Application.RunApplication();
}
