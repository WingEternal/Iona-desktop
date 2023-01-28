QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = core_display
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

CONFIG += c++14

include($$PWD/thirdparty/stb/stb.pri)
include($$PWD/thirdparty/cubism/cubism.pri)
include($$PWD/iona_gl/iona_gl.pri)
include($$PWD/voice/voice.pri)
include($$PWD/app/app.pri)
include($$PWD/tray/tray.pri)
include($$PWD/move_ctrl/move_ctrl.pri)
win32: include($$PWD/res_monitor/res_monitor.pri)
include($$PWD/quick_cmds/quick_cmds.pri)

RESOURCES += \
    resources/resources.qrc

win32:LIBS += -lOpengl32 -lglu32
unix:LIBS += -lglut -lGLU

# Hook
win32: LIBS += -L$$OUT_PWD/../bin/lib/ -lhook
unix:!macx: LIBS += -L$$OUT_PWD/../bin/lib/ -lhook
INCLUDEPATH += $$PWD/../hook
DEPENDPATH += $$PWD/../hook

RC_ICONS = Core.ico

DESTDIR = ../bin
