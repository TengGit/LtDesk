/* Copyright (C) 2016 Teng K. J.
   All rights reserved.
*/

#include "BaseWindow.h"
#ifdef NDEBUG
#include <iostream>
#endif

using namespace tl;

class MyWindow: public BaseWindow {
public:
    MyWindow()
private:
    BaseWindow btn;
};

TCHAR szClassName[] = _T("LightDesktopFrame");
TCHAR szHello[] = _T("Hello, World!\nWelcome to use LightDesktop!");
TCHAR szButtonExitName[] = _T("&Exit");
#define WND_NAME _T("Light Desktop")

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
int RegWndCls();

int APIENTRY Main(int nCmdShow) {
    HWND hWndParent;

    if (!RegWndCls())
        return 0;

    hWndParent = FindWindow(_T("ProgMan"), NULL);
    BaseWindow wnd(szClassName, WND_NAME, SizeAndPos(),
                   WS_VISIBLE | WS_OVERLAPPEDWINDOW,
                   0, hWndParent);
    ShowWindow(wnd, SW_SHOWMAXIMIZED);

    return Application.RunApplication();
}

int RegWndCls() {
    WNDCLASSEX cl;

    /* The Window structure */
    cl.hInstance = Application;
    cl.lpszClassName = szClassName;
    cl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    cl.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;  /* Catch double-clicks */
    cl.cbSize = sizeof(WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    cl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    cl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    cl.hCursor = LoadCursor(NULL, IDC_ARROW);
    cl.lpszMenuName = NULL;                 /* No menu */
    cl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    cl.cbWndExtra = 0;                      /* structure or the window instance */

    /* Use Windows's default colour as the background of the window */
    cl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    return RegisterClassEx(&cl);
}

//static void ProcOnCreate(HWND);
static void ProcOnPaint(HWND);
//static void ProcOnResize(HWND);

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
//    std::cout << "Message " << std::hex << message << " on window " << hwnd << std::endl;
    switch (message) {                /* handle the messages */
//    case WM_CREATE:
//        ProcOnCreate(hwnd);
//        break;
    case WM_DESTROY:
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;
    case WM_PAINT:
        ProcOnPaint(hwnd);
        break;
//    case WM_SIZE:
//        ProcOnResize(hwnd);
//        break;
    case WM_COMMAND:
        DestroyWindow(hwnd);
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

//static void ProcOnCreate(HWND hwnd) {
//    std::cout << "Created" << std::endl;
//}

static void ProcOnPaint(HWND hwnd) {
    PAINTSTRUCT ps;
    RECT rect;
    HDC hdc;
    HBRUSH hb = (HBRUSH)GetStockObject(NULL_BRUSH);
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
