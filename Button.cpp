/* Copyright (C) 2016 Teng K. J.
   All rights reserved.
*/
#include "Button.h"

static const TCHAR _0T_BUTTON_CLASS[] = _T("TGUIButton");

using namespace tl;

static ATOM RegButtonClass();
static ATOM ButtonClass = 0;
static LRESULT CALLBACK ButtonProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
static WNDPROC OriginalWndProc = NULL;

Button::Button(): BaseWindow(_0T_BUTTON_CLASS), parent(NULL) {}

Button::Button(BaseWindow *p): BaseWindow(_0T_BUTTON_CLASS), parent(p) {}

Button::Button(BaseWindow *parent, LPCTSTR name, SizeAndPos sp, bool visible):
    BaseWindow(_0T_BUTTON_CLASS) {
    Create(parent, name, sp, visible);
}

Button::~Button() {
    Destroy();
}

int Button::Create(BaseWindow *parent, LPCTSTR name, SizeAndPos sp, bool visible) {
    int style = WS_CHILD, result;
    RegButtonClass();
    if (visible) style |= WS_VISIBLE;
    result = BaseWindow::Create(name, sp, style, 0, *parent, NULL, Application);
    return result;
}

void Button::Destroy() {
    BaseWindow::Destroy();
}

static ATOM RegButtonClass() {
    if (ButtonClass == 0) {
        WNDCLASSEX btnClass;
        GetClassInfoEx(NULL, _T("Button"), &btnClass);
        btnClass.cbSize = sizeof(btnClass);
        btnClass.lpszClassName = _0T_BUTTON_CLASS;
        OriginalWndProc = btnClass.lpfnWndProc;
        btnClass.lpfnWndProc = ButtonProc;
//        btnClass.hInstance = Application;
        ButtonClass = RegisterClassEx(&btnClass);
    }
    return ButtonClass;
}

static LRESULT CALLBACK ButtonProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    tl::WndMgr_t::iterator it = AllWindows.find(hwnd);
    if (it != AllWindows.end()) {
        switch (msg) {
        case WM_LBUTTONUP:
            static_cast<Button *>(it->second)->OnClick();
        }
    }
    return CallWindowProc(OriginalWndProc, hwnd, msg, wParam, lParam);
}
