/* Copyright (C) 2016 Teng K. J.
   All rights reserved.
*/

#include "Window.h"
#ifdef NDEBUG
#include <iostream>
#endif

using namespace tl;

static void ProcOnPaint(HWND);
//static void ProcOnResize(HWND);

//TCHAR szClassName[] = _T("LightDesktopFrame");
TCHAR szHello[] = _T("Hello, World!\nWelcome to use LightDesktop!");
TCHAR szButtonExitName[] = _T("&Exit");
#define WND_NAME _T("Light Desktop")

class MyWindow: public Window {
public:
    MyWindow() {}
    void Create() {
        HWND hWndParent;

        hWndParent = FindWindow(_T("ProgMan"), NULL);
        Window::Create(WND_NAME, SizeAndPos(),
                       WS_VISIBLE | WS_OVERLAPPEDWINDOW,
                       0, hWndParent);
        ShowWindow(*this, SW_SHOWMAXIMIZED);
    }

    int OnEvent(UINT msg, WPARAM wParam, LPARAM lParam) {
        switch (msg) {
        case WM_PAINT:
            ProcOnPaint(*this);
            break;
        default:
            return DefWindowProc(*this, msg, wParam, lParam);
        }
        return 0;
    }
private:
//    BaseWindow btn;
};

int APIENTRY Main(int nCmdShow) {
    MyWindow wnd;

    wnd.Create();

    return Application.RunApplication();
}

static void ProcOnPaint(HWND hwnd) {
    PAINTSTRUCT ps;
    RECT rect;
    HDC hdc;
    HBRUSH hb = (HBRUSH)GetStockObject(DC_BRUSH);
    HFONT hf = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
    int height;
    hdc = BeginPaint(hwnd, &ps);
    SelectObject(hdc, hb);
    SelectObject(hdc, hf);
    GetClientRect(hwnd, &rect);
    height = DrawText(hdc, szHello, -1, &rect, DT_CALCRECT);
    GetClientRect(hwnd, &rect);
    rect.top += (rect.bottom - rect.top - height) / 2;
    rect.bottom = rect.top + height;
    DrawText(hdc, szHello, -1, &rect, DT_CENTER);
    EndPaint(hwnd, &ps);
}

//static void ProcOnResize(HWND hwnd) {
//    RECT rect;
//    GetClientRect(hwnd, &rect);
//    MoveWindow(hButtonExit, rect.right - BTN_WIDTH, 0, BTN_WIDTH, BTN_HEIGHT, TRUE);
//}
