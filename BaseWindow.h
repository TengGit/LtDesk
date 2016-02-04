/* Copyright (C) 2016 Teng K. J.
   All rights reserved.
*/
#ifndef _0T_BASEWINDOW_H
#define _0T_BASEWINDOW_H

#include "Defs.h"
#include <map>

namespace tl {

struct Size {
    int Width, Height;
    Size(): Width(CW_USEDEFAULT), Height(CW_USEDEFAULT) {}
    Size(int w, int h): Width(w), Height(h) {}
};
struct Pos {
    int X, Y;
    Pos(): X(CW_USEDEFAULT), Y(CW_USEDEFAULT) {}
    Pos(int x, int y): X(x), Y(y) {}
};
struct SizeAndPos {
    Size size;
    Pos pos;
    SizeAndPos() {}
    SizeAndPos(int x, int y, int w, int h): size(w, h), pos(x, y) {}
};

class BaseWindow {
public:
    /** Default constructor */
    BaseWindow(LPCTSTR ClassName);
    /** Default destructor */
    virtual ~BaseWindow();

    operator HWND() const {return hwnd;}
protected:
    BaseWindow(LPCTSTR ClassName, LPCTSTR windowName, SizeAndPos sp, DWORD style, DWORD exStyle,
               HWND parent = NULL, HMENU menu = NULL, HINSTANCE inst = Application);

    int Create(LPCTSTR windowName, SizeAndPos sp, DWORD style, DWORD exStyle,
               HWND parent = NULL, HMENU menu = NULL, HINSTANCE inst = Application);
    void Destroy();

    HWND hwnd;
private:
    LPCTSTR classname;
};

typedef std::map<HWND, BaseWindow *> WndMgr_t;
extern WndMgr_t AllWindows;

}

#endif // _0T_BASEWINDOW_H
