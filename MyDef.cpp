/* Copyright (C) 2016 Teng K. J.
   All rights reserved.
*/

#include "MyDef.h"

TCHAR szHello[] = _T("Welcome to use LightDesktop!\n\n×¢£ºË«»÷ÍË³ö");
TCHAR helloWorld[] = _T("Hello, world!");
TCHAR szButtonExitName[] = _T("&Exit");

tl::MenuItem items[] = {
    {(int)IDM_SHOWHELLO, 0, _T("&Show \"Hello World\" message"), NULL},
    {(int)IDM_EXIT, 0, _T("&Exit"), NULL},
    {tl::MenuItem::INVALID_MENU_ID, 0, NULL, NULL}
};

int Pause(int millis, bool doEvents) {
    int tmr;
    if (millis <= 0) return 0;
    tmr = GetTickCount();
    if (doEvents) {
        do {
            DoEvents();
        } while (!((GetTickCount() - tmr) >= (UINT)millis));
    } else {
        Sleep(millis);
    }
    return GetTickCount() - tmr;
}

void DoEvents() {
    MSG msg;
    if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
        if (GetMessage(&msg, NULL, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            tl::Application.ExitApplication(msg.wParam);
        }
    }
}

int GetRelativeRect(HWND hwnd, LPRECT rect) {
    if (!GetWindowRect(hwnd, rect)) return 0;

    POINT pt;
    HWND parent = GetParent(hwnd);
    pt.x = rect->left;
    pt.y = rect->top;
    if (!ScreenToClient(parent, &pt)) return 0;
    rect->left = pt.x;
    rect->top = pt.y;

    pt.x = rect->right;
    pt.y = rect->bottom;
    if (!ScreenToClient(parent, &pt)) return 0;
    rect->right = pt.x;
    rect->bottom = pt.y;

    return 1;
}
