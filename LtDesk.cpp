/* Copyright (C) 2016 Teng K. J.
   All rights reserved.
*/

#if 0 && !__GNUC__
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "tgui.h"
#include "MyWindow.h"

using namespace tl;

int APIENTRY Main(int) {
    MyWindow wnd;
    int result;
    CoInitialize(NULL);
    OutputDebugString(_T("Debugging...\n\n\n"));
    wnd.Run();
    result = Application.RunApplication();
	CoUninitialize();
    #if !__GNUC__
//    _CrtDumpMemoryLeaks();
    #endif
    return result;
}
