QT += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

INCLUDEPATH += \
    ./include \

HEADERS += \
    include/core/core_global.h \
    include/core/iona_widget.h \
    include/core/main_widget.h \
    include/core/plugin_interface.h \
    include/core/portrait_widget.h

SOURCES += \
    src/iona_widget.cpp \
    src/main.cpp \
    src/main_widget.cpp \
    src/portrait_widget.cpp

FORMS +=

RESOURCES += \
    assets/resource.qrc

RC_ICONS = Core.ico

DESTDIR = ../bin
