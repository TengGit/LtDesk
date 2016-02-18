/* Copyright (C) 2016 Teng K. J.
   All rights reserved.
*/

#include "tgui.h"
#include "MyWindow.h"

using namespace tl;

int APIENTRY Main(int) {
    MyWindow wnd;
    int result;
    CoInitialize(NULL);
    wnd.Run();
    result = Application.RunApplication();CoUninitialize();
    return result;
}
