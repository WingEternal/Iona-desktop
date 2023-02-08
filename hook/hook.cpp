#include "hook.h"
#include <QDebug>

#ifdef Q_OS_WIN
HHOOK Hook::m_hook = nullptr;
HHOOK Hook::kb_hook = nullptr;
HWND Hook::hwnd = nullptr;
const UINT Hook::WM_IONAGL_LOOKAT = RegisterWindowMessage(L"WM_IONAGL_LOOKAT");

BOOL Hook::InstallMouseHook(HWND hWnd)
{
    if(WM_IONAGL_LOOKAT == 0)
    {
        qDebug() << GetLastError();
        return FALSE;
    }
    m_hook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, GetModuleHandle(NULL), 0);
    hwnd = hWnd;
    if (m_hook != nullptr)
        return TRUE;
    else return FALSE;
}

BOOL Hook::UnInstallMouseHook()
{ return UnhookWindowsHookEx(m_hook); }

BOOL Hook::InstallKeybordHook()
{
    kb_hook = SetWindowsHookEx(WH_KEYBOARD_LL, keyBordProc, GetModuleHandle(NULL), 0);
    if (kb_hook != nullptr)
        return TRUE;
    else return FALSE;
}

BOOL Hook::UnInstallKeybordHook()
{ return UnhookWindowsHookEx(kb_hook); }

LRESULT CALLBACK Hook::MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if(wParam == WM_LBUTTONDOWN || wParam == WM_MOUSEMOVE || wParam == WM_LBUTTONUP)
    {
        MOUSEHOOKSTRUCT *mhookstruct = (MOUSEHOOKSTRUCT*)lParam;
        POINT mpt = mhookstruct->pt;
        switch (wParam)
        {
        case WM_LBUTTONDOWN:
            PostMessage(hwnd, WM_IONAGL_LOOKAT, WM_LBUTTONDOWN, MAKELPARAM(mpt.x, mpt.y));
            break;
        case WM_MOUSEMOVE:
            PostMessage(hwnd, WM_IONAGL_LOOKAT, WM_MOUSEMOVE, MAKELPARAM(mpt.x, mpt.y));
            break;
        case WM_LBUTTONUP:
            PostMessage(hwnd, WM_IONAGL_LOOKAT, WM_LBUTTONUP, MAKELPARAM(mpt.x, mpt.y));
            break;
        default: break;
        }
    }
    return CallNextHookEx(m_hook, nCode, wParam, lParam);
}

LRESULT CALLBACK Hook::keyBordProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (wParam == WM_KEYDOWN)
    {
        tagKBDLLHOOKSTRUCT *keyhookstruct;   //键盘HOOK结构体
        keyhookstruct = (tagKBDLLHOOKSTRUCT*)lParam;
        PostMessage(hwnd, WM_KEYDOWN, keyhookstruct->vkCode, keyhookstruct->scanCode);
    }
    return CallNextHookEx(kb_hook, nCode, wParam, lParam);
}
#endif
