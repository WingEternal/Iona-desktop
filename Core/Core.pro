QT += core gui widgets multimedia

CONFIG += c++11

INCLUDEPATH += \
    ./include \

HEADERS += \
    include/main_widget/iona_widget.h \
    include/main_widget/main_widget.h \
    include/plugin_core/plugin_interface.h \
    include/plugin_core/plugin_manager.h

SOURCES += \
    src/iona_widget.cpp \
    src/main.cpp \
    src/main_widget.cpp \
    src/plugin_manager.cpp

FORMS += \
    iona_widget.ui \
    main_widget.ui

RESOURCES += \
    assets/resource.qrc
