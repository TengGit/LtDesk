/* Copyright (C) 2016 Teng K. J.
   All rights reserved.
*/
#include "BaseWindow.h"

using namespace tl;

BaseWindow::BaseWindow(LPCSTR ClassName): hwnd(NULL), classname(ClassName) {}

BaseWindow::BaseWindow(LPCSTR ClassName, LPCSTR windowName, SizeAndPos sp,
                       DWORD style, DWORD exStyle,
                       HWND parent, HMENU menu,
                       HINSTANCE inst): hwnd(NULL), classname(ClassName) {
    Create(windowName, sp, style, exStyle, parent, menu, inst);
}

BaseWindow::~BaseWindow() {
    if (hwnd) {
        Destroy();
    }
}

int BaseWindow::Create(LPCSTR windowName, SizeAndPos sp,
                       DWORD style, DWORD exStyle,
                       HWND parent, HMENU menu,
                       HINSTANCE hInst) {
    if (hwnd == NULL) {
        hwnd = CreateWindowEx(exStyle, classname, windowName, style,
                              sp.pos.X, sp.pos.X, sp.size.Height, sp.size.Width,
                              parent, menu, hInst, NULL);
        if (hwnd == NULL)
            return 0;
        else
            return 1;
    } else {
        return 0;
    }
}

void BaseWindow::Destroy() {
    if (hwnd != NULL) {
        DestroyWindow(hwnd);
        hwnd = NULL;
    }
}
