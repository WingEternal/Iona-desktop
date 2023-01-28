QT  += opengl

INCLUDEPATH += \
    $$PWD

HEADERS += \
    $$PWD/gl_handle.h \
   $$PWD/gl_object_base.h \
   $$PWD/gl_widget.h \
    $$PWD/l2d.h \
    $$PWD/l2d_config.h \
    $$PWD/l2d_model.h \
    $$PWD/l2d_tex_manager.h \
    $$PWD/l2d_touch_manager.h \
    $$PWD/data_ring.h \
    $$PWD/l2d_utils.h

SOURCES += \
    $$PWD/data_ring_model.cpp \
    $$PWD/gl_handle.cpp \
    $$PWD/gl_widget.cpp \
    $$PWD/l2d.cpp \
    $$PWD/l2d_allocator.cpp \
    $$PWD/l2d_model.cpp \
    $$PWD/l2d_pal.cpp \
    $$PWD/l2d_tex_manager.cpp \
    $$PWD/l2d_touch_manager.cpp \
    $$PWD/data_ring.cpp \
