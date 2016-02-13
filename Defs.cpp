/* Copyright (C) 2016 Teng K. J.
   All rights reserved.
*/

#include "Defs.h"

namespace tl {
    App Application;
}

using tl::App;
using tl::Application;

int APIENTRY WinMain(HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     CmdLine,
                     int       nCmdShow) {
    Application.hThis = hThisInstance;
    Application.hPrev = hPrevInstance;
    return Main(nCmdShow);
}

int App::RunApplication() {
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

void App::ExitApplication(int state) {
    PostThreadMessage(GetCurrentThreadId(), WM_QUIT, state, 0);
}
