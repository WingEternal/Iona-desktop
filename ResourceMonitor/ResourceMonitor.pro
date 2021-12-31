#-------------------------------------------------
#
# Project created by QtCreator 2021-12-29T21:14:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = lib
DEFINES += RESOURCE_MONITOR_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

INCLUDEPATH += \
    ./include \
    ./../Core/include

SOURCES += \
    src/hexagon_widget.cpp \
    src/monitor_realization.cpp \
    src/monitor_widget.cpp \
    src/rm_entry.cpp

HEADERS += \
    ../Core/include/core/plugin_interface.h \
    include/resource_monitor/hexagon_widget.h \
    include/resource_monitor/monitor_realization.h \
    include/resource_monitor/monitor_widget.h \
    include/resource_monitor/rm_global.h

FORMS += \
    monitor_widget.ui

DESTDIR = ../bin/lib

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
