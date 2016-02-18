/* Copyright (C) 2016 Teng K. J.
   All rights reserved.
*/

#include "Menu.h"

#ifdef UNICODE
#include <wchar.h>
#define strlen wcslen
#else
#include <string.h>
#endif // UNICODE

using namespace tl;

Menu::Menu(): hMenu(CreatePopupMenu()), needDestroy(true) {}

Menu::Menu(const MenuItem *item): hMenu(CreatePopupMenu()), needDestroy(true) {
    AppendItems(item);
}

Menu::Menu(HMENU hM): hMenu(hM), needDestroy(false) {}

Menu::~Menu() {
    if (hMenu && needDestroy) DestroyMenu(hMenu);
}

static void MenuItem2MII(MENUITEMINFO *mii, const MenuItem *mi);

void Menu::AppendItem(const MenuItem *item) {
    if (item->ID != INVALID_MENU_ID) {
        MENUITEMINFO mii;
        MenuItem2MII(&mii, item);
        InsertMenuItem(hMenu, GetMenuItemCount(hMenu), MF_BYPOSITION, &mii);
    }
}

void Menu::AppendItems(const MenuItem *items) {
    int pos = 0, startPos = GetMenuItemCount(hMenu);
    while (items[pos].ID != INVALID_MENU_ID) {
        if (items[pos].subMenu != NULL) {
            Menu submenu(CreatePopupMenu());
            MENUITEMINFO mii;
            submenu.AppendItems(items[pos].subMenu);
            MenuItem2MII(&mii, items + pos);
            mii.fMask |= MIIM_SUBMENU;
            mii.hSubMenu = submenu;
            InsertMenuItem(hMenu, startPos + pos, MF_BYPOSITION, &mii);
        } else {
            AppendItem(items + pos);
        }
        pos++;
    }
}

void Menu::InsertItem(const MenuItem *item, int pos) {
    if (item->ID != INVALID_MENU_ID) {
        MENUITEMINFO mii;
        MenuItem2MII(&mii, item);
        InsertMenuItem(hMenu, pos, MF_BYPOSITION, &mii);
    }
}

void Menu::InsertItems(const MenuItem *items, int startPos) {
    int pos = 0;
    while (items[pos].ID != INVALID_MENU_ID) {
        if (items[pos].subMenu != NULL) {
            Menu submenu(CreatePopupMenu());
            MENUITEMINFO mii;
            submenu.AppendItems(items[pos].subMenu);
            mii.fMask |= MIIM_SUBMENU;
            mii.hSubMenu = submenu;
            InsertMenuItem(hMenu, startPos + pos, MF_BYPOSITION, &mii);
        } else {
            InsertItem(items + pos, startPos + pos);
        }
        pos++;
    }
}

void Menu::DeleteItem(int ID) {
    DeleteMenu(hMenu, ID, MF_BYCOMMAND);
}

int Menu::Popup(Pos pos, BaseWindow *wnd) const {
    POINT mouse;
    POINT used;
    used.x = pos.X;
    used.y = pos.Y;
    if (used.x == CW_USEDEFAULT || used.y == CW_USEDEFAULT) {
        GetCursorPos(&mouse);
        if (used.x == CW_USEDEFAULT)
            used.x = mouse.x;
        if (used.y == CW_USEDEFAULT)
            used.y = mouse.y;
    }
    return TrackPopupMenu(hMenu, TPM_RETURNCMD |TPM_NONOTIFY, used.x, used.y, 0, *wnd, NULL);
}

Menu Menu::operator [] (int pos) const {
    return Menu(GetSubMenu(hMenu, pos));
}

static void MenuItem2MII(MENUITEMINFO *mii, const MenuItem *mi) {
    mii->cbSize = sizeof(MENUITEMINFO);
    mii->fMask = MIIM_FTYPE | MIIM_ID | MIIM_STATE | MIIM_STRING;

    mii->fType = 0;
    mii->fState = 0;
    mii->wID = mi->ID;
    #define MOVEBIT(a,t,b) ((mi->style & MenuItem::a)?mii->t|=(b):0)
    #ifdef MFS_GRAYED
    #undef MFS_GRAYED
    #endif
    #ifdef MFS_DISABLED
    #undef MFS_DISABLED
    #endif
    #define MFS_GRAYED   0x1
    #define MFS_DISABLED 0x2
    MOVEBIT(BITMAP_MENU, fType, MFT_BITMAP);
    MOVEBIT(SEPARATOR, fType, MFT_SEPARATOR);
    MOVEBIT(BREAK_HERE, fType, MFT_MENUBREAK);
    MOVEBIT(BREAK_HERE_WITH_SEPARATOR, fType, MFT_MENUBARBREAK);
    MOVEBIT(RADIO_CHECK, fType, MFT_RADIOCHECK);
    MOVEBIT(RTL_READING, fType, MFT_RIGHTORDER);
    MOVEBIT(RIGHT_JUSTIFY, fType, MFT_RIGHTJUSTIFY);

    MOVEBIT(CHECKED, fState, MFS_CHECKED);
    MOVEBIT(DEFAULT_MENU, fState, MFS_DEFAULT);
    MOVEBIT(DISABLED, fState, MFS_DISABLED);
    MOVEBIT(GRAYED, fState, MFS_GRAYED);
    MOVEBIT(HIGHLIGHTED, fState, MFS_CHECKED);

    mii->dwTypeData = (LPTSTR)mi->menuData;
    if (!(mi->style & MenuItem::BITMAP_MENU) && mi->menuData != NULL) {
        mii->cch = strlen((LPTSTR)mi->menuData);
    }
    if (mi->hBmpChecked || mi->hBmpUnchecked) {
        mii->fMask |= MIIM_CHECKMARKS;
        mii->hbmpChecked = mi->hBmpChecked;
        mii->hbmpUnchecked = mi->hBmpUnchecked;
    }
}
