/* Copyright (C) 2016 Teng K. J.
   All rights reserved.
*/

#include "Menu.h"

using namespace tl;

const int MenuItem::INVALID_MENU_ID = -1;

Menu::Menu(): hMenu(CreatePopupMenu()) {}

Menu::Menu(const MenuItem *item): hMenu(CreatePopupMenu()) {
    AppendItems(item);
}

Menu::Menu(HMENU hM): hMenu(hM) {}

Menu::~Menu() {
    if (hMenu) DestroyMenu(hMenu);
}

void Menu::AppendItem(const MenuItem *item) {
    if (item->ID != MenuItem::INVALID_MENU_ID) {
        AppendMenu(hMenu, item->style, item->ID, item->menuString);
    }
}

void Menu::AppendItems(const MenuItem *items) {
    int pos = 0;
    while (items[pos].ID != MenuItem::INVALID_MENU_ID) {
        AppendMenu(hMenu, items[pos].style, items[pos].ID, items[pos].menuString);
        if (items[pos].subMenu != NULL) {
            Menu submenu(GetSubMenu(hMenu, pos));
            submenu.AppendItems(items[pos].subMenu);
        }
        pos++;
    }
}
/*
void Menu::InsertItem(const MenuItem *item, int pos) {
    if (item->ID != INVALID_MENU_ID) {
        InsertMenuItem(hMenu, item->style, item->ID, item->menuString);
    }
}
*/

void Menu::DeleteItem(int ID) {
    DeleteMenu(hMenu, ID, MF_BYCOMMAND);
}

int Menu::Popup(Pos pos, BaseWindow *wnd) {
    POINT mouse;
    POINT used;
    used.x = pos.X;
    used.y = pos.Y;
    if (used.x == CW_USEDEFAULT || used.y == CW_USEDEFAULT)
        GetCursorPos(&mouse);
    if (used.x == CW_USEDEFAULT)
        used.x = mouse.x;
    if (used.y == CW_USEDEFAULT)
        used.y = mouse.y;
    return TrackPopupMenu(hMenu, TPM_RETURNCMD |TPM_NONOTIFY, used.x, used.y, 0, *wnd, NULL);
}
