#ifndef SIDEBAR_H_INCLUDED
#define SIDEBAR_H_INCLUDED

/* Copyright (C) 2016 Teng K. J.
   All rights reserved.
*/

#include "MyWindow.h"

class MyWindow;

class SideBar: public Window {
private:
    enum {
        SIDEBAR_LEN = 3,
        SIDEBAR_WIDTH = 64,
        TIMEOUT_MOVE = 50, /* unit: ms */
        PIXEL_PER_MOVE = 1
    };
    MyWindow *parent;
    enum state {NORMAL, ENTERING, LEAVING} eventState;
public:
    SideBar(): parent(NULL), eventState(NORMAL) {}
    void Create(MyWindow *p) {
        RECT parentArea;
        parent = p;
        GetClientRect(*p, &parentArea);
        parentArea.bottom -= parentArea.top;
        parentArea.right -= parentArea.left;
        Window::Create(NULL, SizeAndPos(-SIDEBAR_WIDTH + SIDEBAR_LEN, 0,
                                        SIDEBAR_WIDTH, parentArea.bottom),
                       WS_VISIBLE | WS_CHILD, 0, *p);
    }
    int OnEvent(UINT msg, WPARAM wParam, LPARAM lParam) {
        switch (msg) {
        case WM_MOUSEMOVE:
            OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam),
                        wParam & MOUSE_STATE_MASK);
            break;
        case WM_MOUSEHOVER:
            OnEnter();
            break;
        case WM_MOUSELEAVE:
            OnLeave();
            break;
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
private:
    void OnEnter() {
        RECT area;
        if (eventState != ENTERING) {
            eventState = ENTERING;
            GetRelativeRect(hwnd, &area);
            while (area.left < 0) {
                if (eventState != ENTERING) return;
                SetWindowPos(hwnd, NULL, area.left, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
                Pause(TIMEOUT_MOVE, true);
                area.left += PIXEL_PER_MOVE;
            }
            SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
            eventState = NORMAL;
        }
    }
    void OnLeave() {
        RECT area;
        if (eventState != LEAVING) {
            eventState = LEAVING;
            GetRelativeRect(hwnd, &area);
            while (area.left > -SIDEBAR_WIDTH + SIDEBAR_LEN) {
                if (eventState != LEAVING) return;
                SetWindowPos(hwnd, NULL, area.left, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
                Pause(TIMEOUT_MOVE, true);
                area.left -= PIXEL_PER_MOVE;
            }
            SetWindowPos(hwnd, NULL, -SIDEBAR_WIDTH + SIDEBAR_LEN, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
            eventState = NORMAL;
        }
    }
    void OnMouseMove(int x, int y, int MouseState) {
        if (GetCapture() != hwnd) {
            SetCapture(hwnd);
            OnEnter();
        } else if (MouseState == 0) {
            RECT area;
            POINT pt;
            GetClientRect(hwnd, &area);
            pt.x = x;
            pt.y = y;
            if (!PtInRect(&area, pt)) {
                ReleaseCapture();
                OnLeave();
            }
        }
    }
};

#endif // SIDEBAR_H_INCLUDED
