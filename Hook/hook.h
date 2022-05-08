﻿#ifndef HOOK_H
#define HOOK_H

#include "hook_global.h"

#ifdef Q_OS_WIN
#include <windows.h>
//#include "stdafx.h"
#pragma comment  (lib, "User32.lib")
#endif

class HOOKSHARED_EXPORT Hook
{
private:
    Hook();

#ifdef Q_OS_WIN
public:
    static BOOL InstallMouseHook(HWND hWnd);
    static BOOL UnInstallMouseHook();

    static BOOL InstallKeybordHook();
    static BOOL UnInstallKeybordHook();

    static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK keyBordProc(int nCode, WPARAM wParam, LPARAM lParam);
private:
    static HHOOK mouse_hook;
    static HHOOK kb_hook;
    static HWND hwnd;
#endif
};

#endif // HOOK_H

