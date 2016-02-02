/* Copyright (C) 2016 Teng K. J.
   All rights reserved.
*/

#include "tgui.h"
#include "ExtWinDef.h"
#include "resource.h"
#include "tdebug.h"

#include <shlobj.h>

using namespace tl;

//static void ProcOnResize(HWND);

//TCHAR szClassName[] = _T("LightDesktopFrame");
TCHAR szHello[] = _T("Welcome to use LightDesktop!\n\n×¢£ºË«»÷ÍË³ö");
TCHAR szButtonExitName[] = _T("&Exit");
#define WND_NAME _T("Light Desktop")

class MyWindow: public Window {
public:
    MyWindow() {}
    void Create() {
        HWND hWndParent;
        RECT rect;

        hWndParent = FindWindow(_T("Progman"), NULL);
        TCHAR nameWallPaper[MAX_PATH + 1];
        SystemParametersInfo(SPI_GETDESKWALLPAPER, MAX_PATH, nameWallPaper, 0);
        SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
        rect.bottom -= rect.top;
        rect.right -= rect.left;
        if (nameWallPaper[0]) {
            hBmpBackground = (HBITMAP) LoadImage(
                NULL, nameWallPaper, IMAGE_BITMAP, rect.right, rect.bottom, LR_LOADFROMFILE);
        } else {
            hBmpBackground = LoadBitmap(Application, (LPCTSTR)IDB_BACKGROUND);
        }

        Window::Create(WND_NAME, SizeAndPos(rect.left, rect.top, rect.right, rect.bottom),
                       WS_VISIBLE | WS_POPUP, 0, hWndParent);
        ShowWindow(hwnd, SW_SHOW);
    }

    void OnDestroy() {
        if (hBmpBackground) {
            DeleteObject((HGDIOBJ)hBmpBackground);
        }
    }

    int OnEvent(UINT msg, WPARAM wParam, LPARAM lParam) {
        DBG(std::cout << WMTranslator.find(msg)->second);
        switch (msg) {
        case WM_PAINT:
            ProcOnPaint();
            break;
        case WM_LBUTTONDBLCLK:
            Destroy();
            break;
        default:
            std::cout << std::endl;
            return DefWindowProc(*this, msg, wParam, lParam);
        }
        std::cout << std::endl;
        return 0;
    }
private:
//    BaseWindow btn;
    HBITMAP hBmpBackground;
    void ProcOnPaint();
    int ProcOnCalcSize(bool calc, LPARAM lParam);
};

int APIENTRY Main(int nCmdShow) {
    MyWindow wnd;

    wnd.Create();

    return Application.RunApplication();
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
