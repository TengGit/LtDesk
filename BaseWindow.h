/* Copyright (C) 2016 Teng K. J.
   All rights reserved.
*/
#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <windows.h>

namespace tl {

class BaseWindow {
public:
    /** Default constructor */
    BaseWindow(TCHAR *ClassName);
    /** Default destructor */
    virtual ~BaseWindow();

    int Create();
    int Destroy();

protected:
    HWND hwnd;
private:
    WNDPROC proc;
    TCHAR *classname;
};

}

#endif // BASEWINDOW_H
