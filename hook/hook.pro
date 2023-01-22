QT       += core

TARGET = hook
TEMPLATE = lib

DEFINES += HOOK_LIBRARY

DEFINES += QT_DEPRECATED_WARNINGS
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
        hook.cpp

HEADERS += \
        hook.h

DESTDIR = ../bin/lib

unix {
    target.path = /usr/lib
    INSTALLS += target
}
