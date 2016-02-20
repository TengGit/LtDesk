/* Copyright (C) 2016 Teng K. J.
   All rights reserved.
*/

#include "MyDef.h"

TCHAR szHello[] = _T("欢迎使用 清爽桌面！");
TCHAR helloWorld[] = _T("Hello, world!");

TCHAR szShowHello[] = _T("显示 \"&Hello World\" 对话框"),
      szSubMenu[] = _T("二级菜单"),
      szDisabledMenu[] = _T("禁用的"),
      szGrayedMenu[] = _T("灰掉的"),
      szLoading[] = _T("加载中..."),
      szNoFile[] = _T("(无文件)"),
      szExit[] = _T("退出(&E)");

tl::MenuItem itemHello[] = {
    {(int)IDM_SHOWHELLO, 0, szShowHello, NULL, NULL, NULL},
    MENU_SEPARATOR,
    {DYN_MENU_BEGIN + 1, tl::MenuItem::GRAYED, szGrayedMenu, NULL, NULL, NULL},
    {DYN_MENU_BEGIN + 2, tl::MenuItem::DISABLED, szDisabledMenu, NULL, NULL, NULL},
    MENU_END
};
tl::MenuItem items[] = {
    {(int)IDM_SUBMENU, 0, szSubMenu, NULL, NULL, itemHello},
    MENU_SEPARATOR,
    {(int)IDM_EXIT, 0, szExit, NULL, NULL, NULL},
    MENU_END
};

tl::MenuItem loading[] = {
    {0, 0, szLoading, NULL, NULL, NULL},
    MENU_END
};

tl::MenuItem nofile[] = {
    {0, tl::MenuItem::GRAYED | tl::MenuItem::DISABLED, szNoFile, NULL, NULL, NULL},
    MENU_END
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

bool PointInSP(const POINT&pt, const tl::SizeAndPos&sp) {
    RECT rect;
    rect.left   = sp.pos.X;
    rect.top    = sp.pos.Y;
    rect.right  = rect.left + sp.size.Width;
    rect.bottom = rect.top + sp.size.Height;
    return PtInRect(&rect, pt);
}
