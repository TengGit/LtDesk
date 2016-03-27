/* Copyright (C) 2016 Teng K. J.
   All rights reserved.
*/

#include "MyDef.h"
#include <stdlib.h>

TCHAR regProgRoot[] = _T("SOFTWARE\\LtDesk");
TCHAR regBootValueName[] = _T("StartupOnBoot");
TCHAR msgRegSetFail[] = _T("设置失败！");
TCHAR msgInstance[] = _T("另一个“清爽桌面”已经运行。");

TCHAR szRunOnBoot[] = _T("开机启动"),
      szLoading[] = _T("加载中..."),
      szNoFile[] = _T("(无文件)"),
      szExit[] = _T("退出(&E)");

tl::MenuItem items[] = {
    {(int)IDM_RUNONBOOT, 0, szRunOnBoot, NULL, NULL, NULL},
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

HBITMAP IconToBitmap(HICON source) {
    HDC hdc = CreateCompatibleDC(NULL), hdcSrc = CreateCompatibleDC(NULL);
    HBITMAP hBitmap;
    ICONINFO ii;

    GetIconInfo(source, &ii);
    hBitmap = CreateCompatibleBitmap(hdc, ii.xHotspot * 2, ii.yHotspot * 2);
    SelectObject(hdc, hBitmap);

    SelectObject(hdcSrc, ii.hbmColor);
    BitBlt(hdc, 0, 0, ii.xHotspot * 2, ii.yHotspot * 2, hdcSrc, 0, 0, SRCCOPY);
    SelectObject(hdcSrc, ii.hbmMask);
    BitBlt(hdc, 0, 0, ii.xHotspot * 2, ii.yHotspot * 2, hdcSrc, 0, 0, SRCCOPY);

    DeleteDC(hdc);
    DeleteDC(hdcSrc);
    return hBitmap;
}

#ifdef UNICODE
#define strcmp wcscmp
#define strncmp wcsncmp
#endif // UNICODE
bool ValueInRegistry(HKEY hKey, LPCTSTR valueName) {
    if (RegQueryValueEx(hKey, valueName, NULL, NULL, NULL, NULL) == 0)
        return true;
    return false;
}
