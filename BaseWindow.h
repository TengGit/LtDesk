/* Copyright (C) 2016 Teng K. J.
   All rights reserved.
*/
#ifndef _0T_BASEWINDOW_H
#define _0T_BASEWINDOW_H

#include "Defs.h"

namespace tl {

struct Size {
    int Height, Width;
    Size(): Height(CW_USEDEFAULT), Width(CW_USEDEFAULT) {}
    Size(int h, int w): Height(h), Width(w) {}
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
    SizeAndPos(int x, int y, int h, int w): size(h, w), pos(x, y) {}
};

class BaseWindow {
public:
    /** Default constructor */
    BaseWindow(LPCSTR ClassName);
    BaseWindow(LPCSTR ClassName, LPCSTR windowName, SizeAndPos sp, DWORD style, DWORD exStyle,
               HWND parent = NULL, HMENU menu = NULL, HINSTANCE inst = Application);
    /** Default destructor */
    virtual ~BaseWindow();

    int Create(LPCSTR windowName, SizeAndPos sp, DWORD style, DWORD exStyle,
               HWND parent = NULL, HMENU menu = NULL, HINSTANCE inst = Application);
    void Destroy();

    operator HWND() const {return hwnd;}

protected:
    HWND hwnd;
private:
    const TCHAR *classname;
};

}

#endif // _0T_BASEWINDOW_H
