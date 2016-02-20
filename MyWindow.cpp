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

using tl::Application;
using tl::Pos;
using tl::MenuItem;

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
    GCC(trace(WMTranslator.find(msg)->second));
    switch (msg) {
    case WM_PAINT:
        ProcOnPaint();
        break;
    case WM_RBUTTONUP:
        switch (menu.Popup(Pos(), this)) {
        case IDM_SHOWHELLO:
            MessageBox(*this, helloWorld, helloWorld, 0);
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
    HFONT hf = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
    int height;

    GetClientRect(hwnd, &rect);
    BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
           hBmpDC, 0, 0, SRCCOPY);

    SelectObject(hdc, hf);
    height = DrawText(hdc, szHello, -1, &rect, DT_CALCRECT);
    GetClientRect(hwnd, &rect);
    rect.top += (rect.bottom - rect.top - height) / 2;
    rect.bottom = rect.top + height;
    DrawText(hdc, szHello, -1, &rect, DT_CENTER);

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
                             _T("\\"), NULL, SW_SHOWNORMAL);
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
//                ShellExecute(NULL, _T("open"), _T("explorer.exe"),
//                             _T(""), NULL, SW_SHOWNORMAL);
//                OnLeave();
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

void MyWindow::SideBar::ShowFiles() {
    Menu mFiles;
    MenuItem item[2] = {MENU_END, MENU_END};
    TCHAR buf[MAX_PATH], file[MAX_PATH];
    HANDLE hFind;
    WIN32_FIND_DATA wfd;
    SHFILEINFO sfi;
    std::set<HICON> iconLoaded;
    ICONINFO ii;

    int i = 100;
    SHGetSpecialFolderPath(NULL, buf, CSIDL_DESKTOPDIRECTORY, FALSE);
    strcat(buf, _T("\\*"));
    hFind = FindFirstFile(buf, &wfd);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (wfd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN ||
                strcmp(wfd.cFileName, _T(".")) == 0 ||
                strcmp(wfd.cFileName, _T("..")) == 0) continue;
            strcpy(file, buf);
            file[strlen(file) - 1] = '\0';
            strcat(file, wfd.cFileName);
            SHGetFileInfo(file, 0, &sfi, sizeof(sfi), SHGFI_ICON | SHGFI_SMALLICON);
            iconLoaded.insert(sfi.hIcon);
            GetIconInfo(sfi.hIcon, &ii);
            item[0].menuData = wfd.cFileName;
            item[0].ID = DYN_MENU_BEGIN + i++;
            item[0].style = 0;
            item[0].hBmpChecked = ii.hbmColor;
            item[0].hBmpUnchecked = ii.hbmColor;
            if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                item[0].subMenu = loading;
            } else {
                item[0].subMenu = NULL;
            }
            mFiles.AppendItems(item);
        } while (FindNextFile(hFind, &wfd) != 0);
        FindClose(hFind);
        SetForegroundWindow(hwnd);
        mFiles.Popup(Pos(), this);
        std::set<HICON>::iterator it;
        for (it = iconLoaded.begin(); it != iconLoaded.end(); ++it) {
            DestroyIcon(*it);
        }
    }
}
