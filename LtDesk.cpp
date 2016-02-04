/* Copyright (C) 2016 Teng K. J.
   All rights reserved.
*/
#define WINVER 0x0600
#include "tgui.h"
#include "ExtWinDef.h"
#include "resource.h"
#include "tdebug.h"

#ifndef SPI_GETDESKWALLPAPER
#define SPI_GETDESKWALLPAPER 0x0073
#endif // SPI_GETDESKWALLPAPER

#include <shlobj.h>

using namespace tl;

//static void ProcOnResize(HWND);

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
//        rect.bottom -= rect.top;
//        rect.right -= rect.left;
        if (nameWallPaper[0]) {
            hBmpBackground = (HBITMAP) LoadImage(
                NULL, nameWallPaper, IMAGE_BITMAP, rect.right, rect.bottom, LR_LOADFROMFILE);
        } else {
            hBmpBackground = LoadBitmap(Application, (LPCTSTR)IDB_BACKGROUND);
        }

        Window::Create(WND_NAME, SizeAndPos(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top),
                       WS_VISIBLE | WS_POPUP, 0, hWndParent);
        SetWindowPos(hwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
        ShowWindow(hwnd, SW_SHOW);
    }

    void OnCreate() {
        btnExit.Create(this);
    }

    void OnDestroy() {
        if (hBmpBackground) {
            DeleteObject((HGDIOBJ)hBmpBackground);
        }
    }

    int OnEvent(UINT msg, WPARAM wParam, LPARAM lParam) {
        //DBG(std::cout << WMTranslator.find(msg)->second);
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
    HBITMAP hBmpBackground;
    void ProcOnPaint();
    int ProcOnCalcSize(bool calc, LPARAM lParam);
    class BtnExit: public Button {
    public:
        int Create(BaseWindow *p) {
            parent = p;
            return Button::Create(parent, "&Exit", SizeAndPos(0, 0, 30, 15));
        }
        void OnClick() {
            static_cast<MyWindow *>(parent)->Destroy();
        }
    private:
        BaseWindow *parent;
    } btnExit;
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
