#ifndef MYWINDOW_H_INCLUDED
#define MYWINDOW_H_INCLUDED

/* Copyright (C) 2016 Teng K. J.
   All rights reserved.
*/

#include "MyDef.h"

#include <olectl.h>

class SideBar;

using tl::Window;
using tl::Menu;
using tl::SizeAndPos;

class MyWindow: public Window {
private:
    HBITMAP hBmpBackground;
    Menu menu;
    HDC hBmpDC;
    IPicture *pict;
    IStream *stream;
public:
    MyWindow(): hBmpBackground(NULL), pict(NULL) {}
    void Run();

    void OnCreate();
    void OnDestroy();
    int OnEvent(UINT msg, WPARAM wParam, LPARAM lParam);
private:
    void ProcOnPaint();
    void ProcessRunOnBoot();

    class SideBar: public Window {
    private:
        enum {
            SIDEBAR_LEN    = 3,
            SIDEBAR_WIDTH  = 64,
            TIMEOUT_MOVE   = 10, /* unit: ms */
            PIXEL_PER_MOVE = 5
        };
        MyWindow *parent;
        enum {NORMAL, ENTERING, LEAVING} eventState;
        enum {MYCOMPUTER, NETWORK, RECYCLEBIN, ALLFILES, EXIT, NUM_RECTS};
        enum {
            IDI_MYCOMPUTER = -16,
            IDI_NETWORK    = -18,
            IDI_RECYCLEBIN = -32,
            IDI_ALLFILES   = -35,
            IDI_EXIT       = -28
        };
        struct icon {
            HICON hIcon;
            tl::SizeAndPos rect;
        } IconRect[NUM_RECTS];
    public:
        SideBar(): parent(NULL), eventState(NORMAL) {}
        void Create(MyWindow *p);
        void OnDestroy();
        int OnEvent(UINT msg, WPARAM wParam, LPARAM lParam);
    private:
        void OnEnter();
        void OnLeave();
        void OnPaint();
        void OnLButtonUp(int x, int y, int MouseState);
        void OnLButtonDown(int x, int y, int MouseState);
        void OnMouseMove(int x, int y, int MouseState);
        void OnMenuSelect(int cmd, int flags, HMENU handle);
        void ShowFiles();
        Menu menu;
    } sideBar;
};

#endif // MYWINDOW_H_INCLUDED
