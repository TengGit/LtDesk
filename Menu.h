#ifndef _0T_MENU_H_INCLUDED
#define _0T_MENU_H_INCLUDED

/* Copyright (C) 2016 Teng K. J.
   All rights reserved.
*/

#include "BaseWindow.h"

namespace tl {

const int INVALID_MENU_ID = -1;

struct MenuItem {
    int ID;
    enum MenuItemStyle {
        STRING_MENU   = 0,
        BITMAP_MENU   = 1 << 0,
        SEPARATOR     = 1 << 1,
        BREAK_HERE    = 1 << 2,
        BREAK_HERE_WITH_SEPARATOR = 1 << 3,
        RADIO_CHECK   = 1 << 4,
        RTL_READING   = 1 << 5,
        RIGHT_JUSTIFY = 1 << 6,
        CHECKED       = 1 << 7,
        DEFAULT_MENU  = 1 << 8,
        DISABLED      = 1 << 9,
        GRAYED        = 1 << 10,
        HIGHLIGHTED   = 1 << 11
    };
    int style;

    void *menuData;
    HBITMAP hBmpChecked;
    HBITMAP hBmpUnchecked;
    MenuItem *subMenu;
/*
    MenuItem(): ID(INVALID_MENU_ID) {}
    MenuItem(int menuID, int menuStyle, TCHAR *title, MenuItem *SubMenu):
        ID(menuID), style(menuStyle), menuString(title), subMenu(SubMenu) {}*/
};

#define MENU_END       {tl::INVALID_MENU_ID, 0, NULL, NULL, NULL, NULL}
#define MENU_SEPARATOR {0, tl::MenuItem::SEPARATOR, NULL, NULL, NULL, NULL}
class Menu {
public:
    Menu();
    Menu(const MenuItem *);
    ~Menu();

    void AppendItem (const MenuItem *);
    void AppendItems(const MenuItem *);
    void InsertItem (const MenuItem *, int pos);
    void InsertItems(const MenuItem *, int startPos);
    void DeleteItem(int ID);

    int Popup(Pos pos, BaseWindow *wnd) const;

    Menu operator [] (int pos) const;
    operator HMENU() {return hMenu;}
    operator MenuItem() const;
private:
    HMENU hMenu;
    bool  needDestroy;
    Menu(HMENU);
};

}

#endif // _0T_MENU_H_INCLUDED
