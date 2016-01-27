/* Copyright (C) 2016 Teng K. J.
   All rights reserved.
*/
#include "BaseWindow.h"

using tl::BaseWindow;

BaseWindow::BaseWindow(TCHAR *ClassName): classname(ClassName) {
}

BaseWindow::~BaseWindow() {

}

BaseWindow::Create() {
    hwnd = CreateWindowEx()
}
