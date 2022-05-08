#include "hook.h"
#ifdef Q_OS_WIN
HHOOK Hook::mouse_hook = nullptr;
HHOOK Hook::kb_hook = nullptr;
HWND Hook::hwnd = nullptr;

Hook::Hook()
{
}

BOOL Hook::InstallMouseHook(HWND hWnd)
{
    mouse_hook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, GetModuleHandle(L"myHookMouseAndKeybord"), 0);
        hwnd = hWnd;
        if (mouse_hook != nullptr)
            return TRUE;
        else return FALSE;
}

BOOL Hook::UnInstallMouseHook()
{ return UnhookWindowsHookEx(mouse_hook); }

BOOL Hook::InstallKeybordHook()
{
    kb_hook = SetWindowsHookEx(WH_KEYBOARD_LL, keyBordProc, GetModuleHandle(L"myHookMouseAndKeybord"), 0);
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
        MOUSEHOOKSTRUCT *mhookstruct;   //鼠标HOOK结构体
        mhookstruct = (MOUSEHOOKSTRUCT*)lParam;
        RECT hwnd_rect;
        GetWindowRect(hwnd, &hwnd_rect);
        POINT mpt = mhookstruct->pt;
        // If mouse is out of hwnd
        if(hwnd_rect.top > mpt.y || hwnd_rect.left > mpt.x ||
            hwnd_rect.bottom < mpt.y || hwnd_rect.right < mpt.x)
        {
            switch (wParam) {
            case WM_LBUTTONDOWN:
                PostMessage(hwnd, WM_LBUTTONDOWN, MK_CONTROL, MAKELPARAM(mpt.x, mpt.y));
                break;
            case WM_MOUSEMOVE:
                PostMessage(hwnd, WM_MOUSEMOVE, MK_CONTROL, MAKELPARAM(mpt.x, mpt.y));
                break;
            case WM_LBUTTONUP:
                PostMessage(hwnd, WM_LBUTTONUP, MK_CONTROL, MAKELPARAM(mpt.x, mpt.y));
                break;
            default: break;
            }
        }
    }
    return CallNextHookEx(mouse_hook, nCode, wParam, lParam);
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
