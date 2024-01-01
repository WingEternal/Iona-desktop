#ifndef IONADESKTOP_HOOK_H
#define IONADESKTOP_HOOK_H

#include <QtCore/qglobal.h>

#if defined(HOOK_LIBRARY)
#define HOOKSHARED_EXPORT Q_DECL_EXPORT
#else
#define HOOKSHARED_EXPORT Q_DECL_IMPORT
#endif

#ifdef Q_OS_WIN
#include "Windows.h"
#include "windowsx.h"
// #include "stdafx.h"
#pragma comment(lib, "User32.lib")
#endif

class HOOKSHARED_EXPORT Hook {
#ifdef Q_OS_WIN
 public:
  static BOOL InstallMouseHook(HWND hWnd);
  static BOOL UnInstallMouseHook();

  static BOOL InstallKeybordHook();
  static BOOL UnInstallKeybordHook();

  static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
  static LRESULT CALLBACK keyBordProc(int nCode, WPARAM wParam, LPARAM lParam);

 private:
  static HHOOK m_hook;
  static HHOOK kb_hook;
  static HWND hwnd;
  static const UINT WM_IONAGL_LOOKAT;
#endif

#ifdef Q_OS_LINUX
  enum EVENT {};
 signals:
  void MouseEvent();
#endif
};

#endif  // IONADESKTOP_HOOK_H
