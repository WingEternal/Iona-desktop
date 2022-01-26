QT += core gui opengl multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

INCLUDEPATH += \
    ./include \

HEADERS += \
    include/core/base_widget.h \
    include/core/core_global.h \
    include/core/gl_widget.h \
    include/core/gl_widget_model.h \
    include/core/hexagon_base.h \
    include/core/move_widget.h \
    include/core/plugin_instance.h \
    include/core/plugin_interface.h

SOURCES += \
    src/base_widget.cpp \
    src/base_widget_config.cpp \
    src/base_widget_gl.cpp \
    src/base_widget_mask.cpp \
    src/base_widget_move.cpp \
    src/base_widget_plugins.cpp \
    src/base_widget_tray.cpp \
    src/base_widget_voice.cpp \
    src/gl_draw.cpp \
    src/gl_models.cpp \
    src/gl_shader.cpp \
    src/gl_texture.cpp \
    src/gl_widget.cpp \
    src/main.cpp \
    src/move_widget.cpp \
    src/plugin_instance.cpp

FORMS +=

win32:LIBS += \
    -lOpengl32 \
    -lglu32
unix:LIBS += -lglut -lGLU

RESOURCES += \
    resources/resources.qrc

RC_ICONS = Core.ico

DESTDIR = ../bin
