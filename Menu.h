#ifndef _0T_MENU_H_INCLUDED
#define _0T_MENU_H_INCLUDED

/* Copyright (C) 2016 Teng K. J.
   All rights reserved.
*/

#include "BaseWindow.h"

namespace tl {

struct MenuItem {
    static const int INVALID_MENU_ID;
    int ID;
    int style;
    TCHAR *menuString;
    MenuItem *subMenu;
/*
    MenuItem(): ID(INVALID_MENU_ID) {}
    MenuItem(int menuID, int menuStyle, TCHAR *title, MenuItem *SubMenu):
        ID(menuID), style(menuStyle), menuString(title), subMenu(SubMenu) {}*/
};

class Menu {
public:
    Menu();
    Menu(const MenuItem *);
    ~Menu();

    void AppendItem (const MenuItem *);
    void AppendItems(const MenuItem *);
//    void InsertItem (const MenuItem *, int pos);
//    void InsertItems(const MenuItem *, int startPos);
    void DeleteItem(int ID);

    int Popup(Pos pos, BaseWindow *wnd);

    Menu& operator [] (int pos);
    operator HMENU() {return hMenu;}
private:
    HMENU hMenu;
    Menu(HMENU);
};

}

#endif // _0T_MENU_H_INCLUDED
