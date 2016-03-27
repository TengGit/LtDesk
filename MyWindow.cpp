/* Copyright (C) 2016 Teng K. J.
   All rights reserved.
*/

#include "MyDef.h"

#include "MyWindow.h"

#include "tdebug.h"
#include <stdlib.h>
#include <shellapi.h>
#include <ole2.h>
#ifdef __GNUC__
#define _WIN32_IE 0x0400
#endif // __GNUC__
#include <shlobj.h>

#include <set>
#include <string>
#ifndef NDEBUG
#include <iostream>
#include <iomanip>
#endif // NDEBUG

#ifdef UNICODE
typedef std::wstring string;
#define strcmp wcscmp
#else
using std::string;
#endif // UNICODE

using std::set;
using std::map;
using std::pair;

using tl::Application;
using tl::Pos;
using tl::MenuItem;

static bool ReadBootInfo();
static bool WriteBootInfo(bool);

void MyWindow::Run() {
    HWND hWndParent;
    RECT rect;

    hWndParent = FindWindow(_T("Progman"), NULL);
//    hWndParent = GetDesktopWindow();
//    hWndParent = HWND_DESKTOP;
    TCHAR nameWallPaper[MAX_PATH + 1];
    SystemParametersInfo(SPI_GETDESKWALLPAPER, MAX_PATH, nameWallPaper, 0);
    SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
    if (nameWallPaper[0]) {
//        hBmpBackground = (HBITMAP) LoadImage(
//            NULL, nameWallPaper, IMAGE_BITMAP, rect.right, rect.bottom, LR_LOADFROMFILE);
        HANDLE hFile = CreateFile(nameWallPaper, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
                                  FILE_ATTRIBUTE_NORMAL, NULL);
        DWORD hwSize, fileSize = GetFileSize(hFile, &hwSize), readSize;
        HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, fileSize);
        LPVOID buf = GlobalLock(hGlobal);
        ReadFile(hFile, buf, fileSize, &readSize, NULL);
        CloseHandle(hFile);
        CreateStreamOnHGlobal(hGlobal, TRUE, &stream);
        GlobalUnlock(hGlobal);
        GCC(
        GUID IID_IPicture;
        CLSIDFromString(L"{7BF80980-BF32-101A-8BBB-00AA00300CAB}", &IID_IPicture);
        )
        OleLoadPicture(stream, fileSize, TRUE, IID_IPicture, (PVOID *)&pict);

        OLE_XSIZE_HIMETRIC width;
        OLE_YSIZE_HIMETRIC height;
        HDC hDC = GetDC(hwnd);
        long cx, cy;
        hBmpDC = CreateCompatibleDC(NULL);
        pict->get_Width(&width);
        pict->get_Height(&height);
        cx = MulDiv(width, GetDeviceCaps(hBmpDC, LOGPIXELSX), HIMETRIC_PER_INCH);
        cy = MulDiv(height, GetDeviceCaps(hBmpDC, LOGPIXELSY), HIMETRIC_PER_INCH);
        hBmpBackground = CreateCompatibleBitmap(hDC, cx, cy);
        ReleaseDC(hwnd, hDC);
        SelectObject(hBmpDC, hBmpBackground);
        pict->Render(hBmpDC, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
                     0, height, width, -height, NULL);
        if (pict)
            pict->Release();
        if (stream)
            stream->Release();
    } else {
        hBmpBackground = LoadBitmap(Application, (LPCTSTR)IDB_BACKGROUND);
        hBmpDC = CreateCompatibleDC(NULL);
        SelectObject(hBmpDC, (HGDIOBJ)hBmpBackground);
    }

    if (ReadBootInfo()) {
        items[0].style |= MenuItem::CHECKED;
    }
    menu.AppendItems(items);
    Window::Create(WND_NAME, SizeAndPos(rect.left, rect.top, rect.right - rect.left DBG(- 100), rect.bottom - rect.top),
                   WS_VISIBLE | WS_POPUP, WS_EX_NOACTIVATE, hWndParent);

    ShowWindow(hwnd, SW_SHOW);
}

void MyWindow::OnCreate() {
    sideBar.Create(this);
}

void MyWindow::OnDestroy() {
    if (hBmpDC)
        DeleteDC(hBmpDC);
    if (hBmpBackground)
        DeleteObject((HGDIOBJ)hBmpBackground);

    sideBar.Destroy();
}
int MyWindow::OnEvent(UINT msg, WPARAM wParam, LPARAM lParam) {
    GCC(trace(WMTranslator[msg]));
    switch (msg) {
    case WM_PAINT:
        ProcOnPaint();
        break;
    case WM_RBUTTONUP:
        switch (menu.Popup(Pos(), this)) {
        case IDM_RUNONBOOT:
            ProcessRunOnBoot();
            break;
        case IDM_EXIT:
            Destroy();
            break;
        }
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void MyWindow::ProcOnPaint() {
    PAINTSTRUCT ps;
    RECT rect;
    HDC hdc = BeginPaint(hwnd, &ps);

    GetClientRect(hwnd, &rect);
    BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
           hBmpDC, 0, 0, SRCCOPY);

    EndPaint(hwnd, &ps);
}

void MyWindow::SideBar::Create(MyWindow *p) {
    RECT parentArea;
    parent = p;
    GetClientRect(*p, &parentArea);
    parentArea.bottom -= parentArea.top;
    parentArea.right -= parentArea.left;

    ExtractIconEx(_T("shell32.dll"), IDI_MYCOMPUTER, &IconRect[MYCOMPUTER].hIcon, NULL, 1);
    ExtractIconEx(_T("shell32.dll"), IDI_NETWORK,    &IconRect[NETWORK].hIcon,    NULL, 1);
    ExtractIconEx(_T("shell32.dll"), IDI_RECYCLEBIN, &IconRect[RECYCLEBIN].hIcon, NULL, 1);
    ExtractIconEx(_T("shell32.dll"), IDI_ALLFILES,   &IconRect[ALLFILES].hIcon,   NULL, 1);
    ExtractIconEx(_T("shell32.dll"), IDI_EXIT,       &IconRect[EXIT].hIcon,       NULL, 1);
    for (int i = 0; i < NUM_RECTS; i++) {
        IconRect[i].rect.pos.X       = 16;
        IconRect[i].rect.pos.Y       = i * 64 + 16;
        IconRect[i].rect.size.Width  = 32;
        IconRect[i].rect.size.Height = 32;
    }
    IconRect[EXIT].rect.pos.Y = parentArea.bottom - 48;

    Window::Create(NULL, SizeAndPos(-SIDEBAR_WIDTH + SIDEBAR_LEN, 0,
                                    SIDEBAR_WIDTH, parentArea.bottom),
                   WS_VISIBLE | WS_CHILD, 0, *p);
}

void MyWindow::SideBar::OnDestroy() {
    for (int i = 0; i < NUM_RECTS; i++) {
        DestroyIcon(IconRect[i].hIcon);
    }
}

int MyWindow::SideBar::OnEvent(UINT msg, WPARAM wParam, LPARAM lParam) {
    GCC(trace("    " << WMTranslator[msg]));
    switch (msg) {
    case WM_PAINT:
        OnPaint();
        break;
    case WM_MOUSEMOVE:
        OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam),
                    wParam & MOUSE_STATE_MASK);
        break;
    case WM_LBUTTONDOWN:
        OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam),
                      wParam & MOUSE_STATE_MASK);
        break;
    case WM_LBUTTONUP:
        OnLButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam),
                    wParam & MOUSE_STATE_MASK);
        break;
    case WM_MENUSELECT:
        OnMenuSelect(LOWORD(wParam), HIWORD(wParam), (HMENU)lParam);
        break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void MyWindow::SideBar::OnEnter() {
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

void MyWindow::SideBar::OnLeave() {
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

void MyWindow::SideBar::OnMouseMove(int x, int y, int MouseState) {
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
        } else {
            POINT p;
            int i;
            p.x = x;
            p.y = y;
            for (i = 0; i < NUM_RECTS; i++) {
                if (PointInSP(p, IconRect[i].rect)) {
                    SetCursor(LoadCursor(NULL, IDC_HAND));
                    break;
                }
            }
            if (i == NUM_RECTS) {
                SetCursor(LoadCursor(NULL, IDC_ARROW));
            }
        }
    }
}

static int LastDownPict;
void MyWindow::SideBar::OnLButtonDown(int x, int y, int MouseState) {
    if (MouseState == MK_LBUTTON) {
        POINT pt;
        pt.x = x;
        pt.y = y;
        for (int i = 0; i < NUM_RECTS; i++) {
            if (PointInSP(pt, IconRect[i].rect)) {
                LastDownPict = i;
                return;
            }
        }
    }
    LastDownPict = -1;
}
void MyWindow::SideBar::OnLButtonUp(int x, int y, int MouseState) {
    POINT pt;
    if (MouseState == 0 && LastDownPict != -1) {
        pt.x = x;
        pt.y = y;
        if (PointInSP(pt, IconRect[LastDownPict].rect)) {
            switch (LastDownPict) {
            case MYCOMPUTER:
                ShellExecute(NULL, _T("open"), _T("explorer.exe"),
                             _T("::{20D04FE0-3AEA-1069-A2D8-08002B30309D}"), NULL, SW_SHOWNORMAL);
                OnLeave();
                break;
            case NETWORK:
                ShellExecute(NULL, _T("open"), _T("explorer.exe"),
                             _T("::{208D2C60-3AEA-1069-A2D7-08002B30309D}"), NULL, SW_SHOWNORMAL);
                OnLeave();
                break;
            case RECYCLEBIN:
                ShellExecute(NULL, _T("open"), _T("explorer.exe"),
                             _T("::{645FF040-5081-101B-9F08-00AA002F954E}"), NULL, SW_SHOWNORMAL);
                OnLeave();
                break;
            case ALLFILES:
                ShowFiles();
                break;
            case EXIT:
                parent->Destroy();
                return;
            }
        }
    }
}

void MyWindow::SideBar::OnPaint() {
    PAINTSTRUCT ps;
    HDC hDC = BeginPaint(hwnd, &ps);

    for (int i = 0; i < NUM_RECTS; i++) {
        DrawIconEx(hDC, IconRect[i].rect.pos.X, IconRect[i].rect.pos.Y, IconRect[i].hIcon,
                   IconRect[i].rect.size.Width, IconRect[i].rect.size.Height, 0, NULL, DI_NORMAL);
    }

    EndPaint(hwnd, &ps);
}

static map<int, string> file_translator;
static set<HICON> iconLoaded;
static map<string, Menu *> menuLoaded;
static int dyn_menu_begin;
static Menu *LoadFileMenu(string dir);
static Menu mFiles;

void MyWindow::SideBar::ShowFiles() {
    TCHAR buf[MAX_PATH];
    string fileName, dirName;
    int result;

    mFiles.Clear();
    SHGetSpecialFolderPath(NULL, buf, CSIDL_DESKTOPDIRECTORY, FALSE);
    dirName = buf;

    dyn_menu_begin = DYN_MENU_BEGIN;
    Menu *mFiles = LoadFileMenu(string(buf));

    SetForegroundWindow(hwnd);
    result = mFiles->Popup(Pos(), this);
    if (result) {
        ShellExecute(NULL, _T("open"), file_translator[result].c_str(), NULL, NULL, SW_SHOWNORMAL);
    }
    trace(result);
    for (std::set<HICON>::iterator it = iconLoaded.begin(); it != iconLoaded.end(); ++it) {
        DestroyIcon(*it);
    }
    for (std::map<string, Menu *>::iterator it2 = menuLoaded.begin();
         it2 != menuLoaded.end();
         ++it2) {
        delete it2->second;
    }
    file_translator.clear();
    iconLoaded.clear();
    menuLoaded.clear();
    OnLeave();
}

void MyWindow::SideBar::OnMenuSelect(int cmd, int flags, HMENU handle) {
    typedef std::map<int, string>::iterator It;
    if (flags == 0xffff || handle == NULL || !(flags & (MF_SYSMENU | MF_POPUP))) return;
    MENUITEMINFO mii;
    BOOL isPos = FALSE;

    DBG(std::cerr << "handle = " << handle << std::endl;)
    mii.cbSize = sizeof(mii);
    mii.fMask = MIIM_SUBMENU | MIIM_ID;
    if (flags & MF_POPUP) isPos = TRUE;
    if (!GetMenuItemInfo(handle, cmd, isPos, &mii)) goto Error;

    if (mii.hSubMenu) {
        int id = mii.wID;
        It it = file_translator.find(id);
        Menu *menu = LoadFileMenu(it->second);
        DeleteMenu(handle, id, MF_BYPOSITION);

        mii.cbSize = sizeof(mii);
        mii.fMask = MIIM_SUBMENU;
        mii.hSubMenu = *menu;
        SetMenuItemInfo(handle, id, FALSE, &mii);
        DrawMenuBar(hwnd);
    }
    DBG(handle = NULL;)
    return;
Error:
    ;
}

static Menu *LoadFileMenu(string dirName) {
    string fileName, buf;
    HANDLE hFind;
    WIN32_FIND_DATA wfd;
    SHFILEINFO sfi;
    MenuItem item[2] = {MENU_END, MENU_END};
    Menu *mFiles = new Menu();
    int LoadedFile = 0;

    if (*dirName.rbegin() != _T('\\')) dirName.append(_T("\\"));
    buf = dirName + _T("*");

    hFind = FindFirstFile(buf.c_str(), &wfd);
    if (hFind != INVALID_HANDLE_VALUE) {
        std::map<string, Menu *>::iterator it = menuLoaded.find(dirName);
        if (it != menuLoaded.end()) {
            return it->second;
        }
        menuLoaded.insert(pair<string, Menu *>(dirName, mFiles));
        do {
            if (wfd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN ||
                strcmp(wfd.cFileName, _T(".")) == 0 ||
                strcmp(wfd.cFileName, _T("..")) == 0) continue;
            LoadedFile = 1;
            fileName = dirName + wfd.cFileName;
            const TCHAR *str = fileName.c_str();

            SHGetFileInfo(str, 0, &sfi, sizeof(sfi), SHGFI_ICON | SHGFI_SMALLICON);
//            HBITMAP hBmp = IconToBitmap(sfi.hIcon);
//            DestroyIcon(sfi.hIcon);
//            bmpLoaded.insert(hBmp);
            iconLoaded.insert(sfi.hIcon);

            ICONINFO ii;
            GetIconInfo(sfi.hIcon, &ii);
            item[0].menuData = wfd.cFileName;
            item[0].ID = DYN_MENU_BEGIN + dyn_menu_begin++;
            item[0].style = 0;
            item[0].hBmpChecked = ii.hbmColor;
            item[0].hBmpUnchecked = ii.hbmColor;
            file_translator.insert(pair<int, string>(item[0].ID, fileName));
            if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                item[0].subMenu = loading;
            } else {
                item[0].subMenu = NULL;
            }
            mFiles->AppendItems(item);
        } while (FindNextFile(hFind, &wfd) != 0);
        FindClose(hFind);

        if (!LoadedFile) {
            goto notFound;
        }
        return mFiles;
    }
notFound:
    mFiles->AppendItem(nofile);
    return mFiles;
}

static bool ReadBootInfo() {
    HKEY kProg = NULL;
    int runOnBoot = 0, sizeOfData = sizeof(int);
    if (RegCreateKeyEx(REG_PROG_ROOT, regProgRoot, 0, NULL, 0, KEY_READ, NULL, &kProg, NULL))
        goto ret;
    RegQueryValueEx(kProg, regBootValueName, NULL, NULL, (LPBYTE)&runOnBoot, (LPDWORD)&sizeOfData);
    if (!runOnBoot)
        goto ret;
    RegCloseKey(kProg);
    runOnBoot = 0;
    if (RegOpenKeyEx(REG_PROG_ROOT, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"),
                     0, KEY_ALL_ACCESS, &kProg))
        goto ret;
    if (!ValueInRegistry(kProg, _T("LtDesk")))
        goto ret;
    runOnBoot = 1;
ret:
    if (kProg) RegCloseKey(kProg);
    return runOnBoot;
}
static bool WriteBootInfo(bool runOnBoot) {
    HKEY kProg = NULL;
    bool succeed = false;
    int len;
    if (RegCreateKeyEx(REG_PROG_ROOT, regProgRoot, 0, NULL, 0, KEY_WRITE, NULL, &kProg, NULL))
        goto ret;
    if (RegSetValueEx(kProg, regBootValueName, 0, REG_DWORD,
                      (const BYTE *)&runOnBoot, sizeof(int)))
        goto ret;
    RegCloseKey(kProg);
    if (RegOpenKeyEx(REG_PROG_ROOT, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"),
                     0, KEY_ALL_ACCESS, &kProg))
        goto ret;
    if (runOnBoot) {
        TCHAR progName[MAX_PATH];
        SetLastError(0);
        len = GetModuleFileName(NULL, progName, sizeof(progName) / sizeof(TCHAR));
        if (!len || GetLastError() == ERROR_INSUFFICIENT_BUFFER)
            goto ret;
        if (RegSetValueEx(kProg, _T("LtDesk"), 0, REG_SZ, (const BYTE *)progName, len))
            goto ret;
    } else {
        RegDeleteValue(kProg, _T("LtDesk"));
    }
    succeed = true;
ret:
    if (kProg) RegCloseKey(kProg);
    return succeed;
}

void MyWindow::ProcessRunOnBoot() {
    MENUITEMINFO mii;
    mii.cbSize = sizeof(mii);
    mii.fMask = MIIM_STATE;
    GetMenuItemInfo(menu, IDM_RUNONBOOT, MF_BYCOMMAND, &mii);
    if (mii.fState & MFS_CHECKED) {
        if (WriteBootInfo(false)) {
            mii.fState &= ~MFS_CHECKED;
            SetMenuItemInfo(menu, IDM_RUNONBOOT, MF_BYCOMMAND, &mii);
            return;
        }
    } else {
        if (WriteBootInfo(true)) {
            mii.fState |= MFS_CHECKED;
            SetMenuItemInfo(menu, IDM_RUNONBOOT, MF_BYCOMMAND, &mii);
            return;
        }
    }

    MessageBox(hwnd, msgRegSetFail, NULL, MB_ICONEXCLAMATION);
}
