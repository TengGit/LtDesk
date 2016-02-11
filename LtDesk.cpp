/* Copyright (C) 2016 Teng K. J.
   All rights reserved.
*/

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
TCHAR helloWorld[] = _T("Hello, world!");
TCHAR szButtonExitName[] = _T("&Exit");
#define WND_NAME _T("Light Desktop")

enum {
    IDM_SHOWHELLO,
    IDM_EXIT,
};
MenuItem items[] = {
    {(int)IDM_SHOWHELLO, 0, _T("&Show \"Hello World\" message"), NULL},
    {(int)IDM_EXIT, 0, _T("&Exit"), NULL},
    {MenuItem::INVALID_MENU_ID, 0, NULL, NULL}
};

int APIENTRY Main(int nCmdShow) {
    class MyWindow: public Window {
    public:
        MyWindow() {}
        void Create() {
            HWND hWndParent;
            RECT rect;

//            hWndParent = FindWindow(_T("Progman"), NULL);
//            hWndParent = GetDesktopWindow();
            hWndParent = HWND_DESKTOP;
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
            Window::Create(WND_NAME, SizeAndPos(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top),
                           WS_VISIBLE | WS_POPUP, WS_EX_NOACTIVATE, hWndParent);

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
            DBG(std::cout << WMTranslator.find(msg)->second << std::endl);
            switch (msg) {
            case WM_PAINT:
                ProcOnPaint();
                break;
            case WM_LBUTTONDBLCLK:
                Destroy();
                break;
            case WM_LBUTTONUP:
                ProcOnClick();
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
//            case WM_WINDOWPOSCHANGING:
//                ((LPWINDOWPOS)lParam)->hwndInsertAfter = HWND_BOTTOM;
//                break;
            default:
                return DefWindowProc(hwnd, msg, wParam, lParam);
            }
            return 0;
        }
    private:
        HBITMAP hBmpBackground;
        Menu menu;
        void ProcOnPaint() {
            PAINTSTRUCT ps;
            RECT rect;
            HDC hdc = BeginPaint(hwnd, &ps), hBmpDC = CreateCompatibleDC(hdc);
            HFONT hf = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
            int height;

//            SetBkMode(hdc, TRANSPARENT);
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
        void ProcOnClick() {
            LONG style = GetWindowLong(hwnd, GWL_STYLE);
            if (style & WS_POPUP) {
                SetWindowLong(hwnd, GWL_STYLE, style & ~WS_POPUP);
            } else {
                SetWindowLong(hwnd, GWL_STYLE, style | WS_POPUP);
            }
        }
        class BtnExit: public Button {
        public:
            int Create(BaseWindow *p) {
                parent = p;
                return Button::Create(parent, szButtonExitName, SizeAndPos(0, 0, 30, 15));
            }
            void OnClick() {
                static_cast<MyWindow *>(parent)->Destroy();
            }
        private:
            BaseWindow *parent;
        } btnExit;
    } wnd;

    wnd.Create();

    return Application.RunApplication();
}
