/* Copyright (C) 2016 Teng K. J.
   All rights reserved.
*/
#ifndef _0T_WINDOW_H
#define _0T_WINDOW_H

#include "BaseWindow.h"

#define _0T_WINDOW_CLASS _T("TGUIWindow")

namespace tl {

class Window: public BaseWindow {
public:
    /** Default constructor */
    Window();
    /** Default destructor */

    int Create(LPCTSTR windowName, SizeAndPos sp, DWORD style, DWORD exStyle,
               HWND parent = NULL, HMENU menu = NULL, HINSTANCE inst = Application);
    void Destroy();

    virtual void OnCreate() {}
    virtual void OnDestroy() {}
    virtual int OnEvent(UINT msg, WPARAM wParam, LPARAM lParam) {
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
};

}

#endif // _0T_WINDOW_H
