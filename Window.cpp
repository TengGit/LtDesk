/* Copyright (C) 2016 Teng K. J.
   All rights reserved.
*/
#include "Window.h"
#include <map>

using namespace tl;

static ATOM RegWndCls();
static ATOM WindowClass = 0;

static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

static std::map<HWND, Window *> WndMgr;

Window::Window(): BaseWindow(_0T_WINDOW_CLASS) {
    //ctor
    RegWndCls();
}

int Window::Create(LPCSTR windowName, SizeAndPos sp, DWORD style, DWORD exStyle,
                   HWND parent, HMENU menu, HINSTANCE inst) {
    int success = BaseWindow::Create(windowName, sp, style, exStyle, parent, menu, inst);
    if (success) {
        WndMgr.insert(std::pair<HWND, Window *>(hwnd, this));
        this->OnCreate();
    }
    return success;
}

void Window::Destroy() {
    return BaseWindow::Destroy();
}

static ATOM RegWndCls() {
    if (!WindowClass) {
        WNDCLASSEX cl;

        /* The Window structure */
        cl.hInstance = Application;
        cl.lpszClassName = _0T_WINDOW_CLASS;
        cl.lpfnWndProc = WndProc;      /* This function is called by windows */
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

        /* Register the window class */
        WindowClass = RegisterClassEx(&cl);
    }
    return WindowClass;
}

static void ProcOnDestroy(HWND hwnd);
static int ProcEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {                /* handle the messages */
    case WM_DESTROY:
        ProcOnDestroy(hwnd);
        break;
    default:                      /* for messages that we don't deal with */
        return ProcEvent(hwnd, message, wParam, lParam);
    }
    return 0;
}

static void ProcOnDestroy(HWND hwnd) {
    std::map<HWND, Window *>::iterator it = WndMgr.find(hwnd);

    if (it != WndMgr.end())
        WndMgr.erase(it);

    if (WndMgr.size() == 0)
        PostQuitMessage(0);       /* send a WM_QUIT to the message queue */
}

static int ProcEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    std::map<HWND, Window *>::iterator it = WndMgr.find(hwnd);

    if (it != WndMgr.end())
        return it->second->OnEvent(msg, wParam, lParam);

    return DefWindowProc(hwnd, msg, wParam, lParam);
}
