QT += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

INCLUDEPATH += \
    ./include \

HEADERS += \
    include/core/core_global.h \
    include/core/iona_widget.h \
    include/core/main_widget.h \
    include/core/plugin_interface.h

SOURCES += \
    src/iona_widget.cpp \
    src/main.cpp \
    src/main_widget.cpp

FORMS += \
    iona_widget.ui \
    main_widget.ui

RESOURCES += \
    assets/resource.qrc

DESTDIR = ../bin
