/* Copyright (C) 2016 Teng K. J.
   All rights reserved.
*/

#include "MyDef.h"

#include "tdebug.h"
#include <stdlib.h>

#include "SideBar.h"
#include "MyWindow.h"

using tl::Application;
using tl::Pos;

void MyWindow::Run() {
    HWND hWndParent;
    RECT rect;

    hWndParent = FindWindow(_T("Progman"), NULL);
//    hWndParent = GetDesktopWindow();
//    hWndParent = HWND_DESKTOP;
    TCHAR nameWallPaper[MAX_PATH + 1];
    SystemParametersInfo(SPI_GETDESKWALLPAPER, MAX_PATH, nameWallPaper, 0);
    SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
    if (nameWallPaper[0]) {
        hBmpBackground = (HBITMAP) LoadImage(
            NULL, nameWallPaper, IMAGE_BITMAP, rect.right, rect.bottom, LR_LOADFROMFILE);
    } else {
        hBmpBackground = LoadBitmap(Application, (LPCTSTR)IDB_BACKGROUND);
    }

    menu.AppendItems(items);
    Window::Create(WND_NAME, SizeAndPos(rect.left, rect.top, rect.right - rect.left DBG(- 100), rect.bottom - rect.top),
                   WS_VISIBLE | WS_POPUP, WS_EX_NOACTIVATE, hWndParent);

    ShowWindow(hwnd, SW_SHOW);
}

void MyWindow::OnCreate() {
    sideBar.Create(this);
}

void MyWindow::OnDestroy() {
    if (hBmpBackground) {
        DeleteObject((HGDIOBJ)hBmpBackground);
    }
    sideBar.Destroy();
}
int MyWindow::OnEvent(UINT msg, WPARAM wParam, LPARAM lParam) {
    GCC(trace(WMTranslator.find(msg)->second));
    switch (msg) {
    case WM_PAINT:
        ProcOnPaint();
        break;
    case WM_LBUTTONDBLCLK:
        Destroy();
        break;
    case WM_RBUTTONUP:
        switch (menu.Popup(Pos(), this)) {
        case IDM_SHOWHELLO:
            MessageBox(*this, helloWorld, helloWorld, 0);
            break;
        case IDM_EXIT:
            Destroy();
            break;
        }
        break;
    case WM_MOUSELEAVE:
        trace("Received WM_MOUSELEAVE");
//    case WM_WINDOWPOSCHANGING:
//        ((LPWINDOWPOS)lParam)->hwndInsertAfter = HWND_BOTTOM;
//        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void MyWindow::ProcOnPaint() {
    PAINTSTRUCT ps;
    RECT rect;
    HDC hdc = BeginPaint(hwnd, &ps), hBmpDC = CreateCompatibleDC(hdc);
    HFONT hf = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
    int height;

//    SetBkMode(hdc, TRANSPARENT);
    SelectObject(hBmpDC, hBmpBackground);
    SelectObject(hdc, hf);
    GetClientRect(hwnd, &rect);

    BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
           hBmpDC, 0, 0, SRCCOPY);
    height = DrawText(hdc, szHello, -1, &rect, DT_CALCRECT);
    GetClientRect(hwnd, &rect);
    rect.top += (rect.bottom - rect.top - height) / 2;
    rect.bottom = rect.top + height;
    DrawText(hdc, szHello, -1, &rect, DT_CENTER);

    DeleteDC(hBmpDC);
    EndPaint(hwnd, &ps);
}
