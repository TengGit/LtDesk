/* Copyright (C) 2016 Teng K. J.
   All rights reserved.
*/

#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
static int RegisterAppClass();

/*  Make the class name into a global variable  */
TCHAR szClassName[] = _T("LightDesktopMain");

TCHAR szHello[] = _T("Hello, World!\nWelcome to use LightDesktop!");
TCHAR szButtonExitName[] = _T("&Exit");

#define CLS_NAME_BUTTON _T("Button")

#define BTN_HEIGHT 15
#define BTN_WIDTH  30
HWND hButtonExit;

int WINAPI WinMain(HINSTANCE hThisInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpszArgument,
                   int nCmdShow) {
    HWND hwnd;               /* This is the handle for our window */
    HWND hwndParent;
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;  /* Catch double-clicks */
    wincl.cbSize = sizeof(WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx(&wincl))
        return 0;

    hwndParent = FindWindow(_T("ProgMan"), NULL);

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx(
               WS_EX_WINDOWEDGE,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               _T("Code::Blocks Template Windows App"),       /* Title Text */
               WS_CHILD,
                                    /* default window */
               CW_USEDEFAULT,       /* Windows decides the position */
               CW_USEDEFAULT,       /* where the window ends up on the screen */
               CW_USEDEFAULT,       /* The programs width */
               CW_USEDEFAULT,       /* and height in pixels */
               hwndParent,          /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               NULL                 /* No Window Creation data */
           );

    hButtonExit = CreateWindowEx(0, CLS_NAME_BUTTON, szButtonExitName,
                                 WS_VISIBLE | WS_CHILD,
                                 0, 0, BTN_WIDTH, BTN_HEIGHT,
                                 hwnd, NULL, hThisInstance, NULL);

    /* Make the window visible on the screen */
    ShowWindow(hwnd, SW_SHOWMAXIMIZED);

    InvalidateRect(GetDesktopWindow(), NULL, TRUE);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage(&messages, NULL, 0, 0)) {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */
static void ProcOnPaint(HWND);
static void ProcOnResize(HWND);
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {                /* handle the messages */
//    case WM_CREATE:
//        void ProcOnCreate(HWND);
//        ProcOnCreate(hwnd);
//        break;
    case WM_DESTROY:
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;
    case WM_PAINT:
        ProcOnPaint(hwnd);
        break;
    case WM_SIZE:
        ProcOnResize(hwnd);
        break;
    case WM_COMMAND:
        DestroyWindow(hwnd);
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

static void ProcOnPaint(HWND hwnd) {
    PAINTSTRUCT ps;
    RECT rect;
    HDC hdc;
    HBRUSH hb = (HBRUSH)GetStockObject(NULL_BRUSH);
    HFONT hf = (HFONT)GetStockObject(SYSTEM_FIXED_FONT);
    int height;
    hdc = BeginPaint(hwnd, &ps);
    SelectObject(hdc, hb);
    SelectObject(hdc, hf);
//    cout << hb << endl;
//    cout << hf << endl;
    GetClientRect(hwnd, &rect);
    height = DrawText(hdc, szHello, -1, &rect, DT_CALCRECT);
    GetClientRect(hwnd, &rect);
    rect.top += (rect.bottom - rect.top - height) / 2;
    rect.bottom = rect.top + height;
    DrawText(hdc, szHello, -1, &rect, DT_CENTER);
    EndPaint(hwnd, &ps);
}

static void ProcOnResize(HWND hwnd) {
    RECT rect;
    GetClientRect(hwnd, &rect);
    MoveWindow(hButtonExit, rect.right - BTN_WIDTH, 0, BTN_WIDTH, BTN_HEIGHT, TRUE);
}
