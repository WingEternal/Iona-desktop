#ifndef HOOK_GLOBAL_H
#define HOOK_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(HOOK_LIBRARY)
#  define HOOKSHARED_EXPORT Q_DECL_EXPORT
#else
#  define HOOKSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // HOOK_GLOBAL_H
