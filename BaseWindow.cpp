/* Copyright (C) 2016 Teng K. J.
   All rights reserved.
*/
#include "BaseWindow.h"

namespace tl {

typedef std::map<HWND, BaseWindow *> WndMgr_t;
WndMgr_t AllWindows;

BaseWindow::BaseWindow(LPCTSTR ClassName): hwnd(NULL), classname(ClassName) {}

BaseWindow::BaseWindow(LPCTSTR ClassName, LPCTSTR windowName, SizeAndPos sp,
                       DWORD style, DWORD exStyle,
                       HWND parent, HMENU menu,
                       HINSTANCE inst): hwnd(NULL), classname(ClassName) {
    Create(windowName, sp, style, exStyle, parent, menu, inst);
}

BaseWindow::~BaseWindow() {
    Destroy();
}

int BaseWindow::Create(LPCTSTR windowName, SizeAndPos sp,
                       DWORD style, DWORD exStyle,
                       HWND parent, HMENU menu,
                       HINSTANCE hInst) {
    if (hwnd == NULL) {
        hwnd = CreateWindowEx(exStyle, classname, windowName, style,
                              sp.pos.X, sp.pos.X, sp.size.Height, sp.size.Width,
                              parent, menu, hInst, NULL);
        if (hwnd == NULL)
            return 0;

        AllWindows.insert(WndMgr_t::value_type(hwnd, this));
    } else {
        return 0;
    }
    return 1;
}

void BaseWindow::Destroy() {
    if (hwnd != NULL) {
        DestroyWindow(hwnd);

        WndMgr_t::iterator it = AllWindows.find(hwnd);
        if (it != AllWindows.end())
            AllWindows.erase(it);

        hwnd = NULL;
    }
}

}
