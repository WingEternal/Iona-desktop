QT += core gui opengl multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

INCLUDEPATH += \
    $$PWD/include \

HEADERS += \
    include/core/base_widget.h \
    include/core/core_global.h \
    include/core/gl_entry.h \
    include/core/gl_object_base.h \
    include/core/gl_widget.h \
    include/core/hexagon_base.h \
    include/core/hitbox_widget.h \
    include/core/move_widget.h \
    include/core/native_event_filter.h \
    include/core/plugin_instance.h \
    include/core/plugin_interface.h \
    include/core_ex1/data_ring.h \
    include/core_ex1/l2d.h \
    include/core_ex1/l2d_allocator.h \
    include/core_ex1/l2d_config.h \
    include/core_ex1/l2d_model.h \
    include/core_ex1/l2d_pal.h \
    include/core_ex1/l2d_tex_manager.h \
    include/core_ex1/l2d_touch_manager.h \
    thirdparty/stb/stb_image.h

SOURCES += \
    src/base_widget.cpp \
    src/base_widget_config.cpp \
    src/base_widget_gl.cpp \
    src/base_widget_hitbox.cpp \
    src/base_widget_mask.cpp \
    src/base_widget_move.cpp \
    src/base_widget_tray.cpp \
    src/base_widget_voice.cpp \
    src/data_ring.cpp \
    src/data_ring_models.cpp \
    src/data_ring_shader.cpp \
    src/data_ring_texture.cpp \
    src/gl_entry.cpp \
    src/gl_widget.cpp \
    src/hitbox_widget.cpp \
    src/l2d.cpp \
    src/l2d_allocator.cpp \
    src/l2d_model.cpp \
    src/l2d_pal.cpp \
    src/l2d_tex_manager.cpp \
    src/l2d_touch_manager.cpp \
    src/main.cpp \
    src/move_widget.cpp \
    src/native_event_filter.cpp \
    src/plugin_instance.cpp \

win32:LIBS += -lOpengl32 -lglu32
unix:LIBS += -lglut -lGLU

# Cubism
CUBISM_PATH = $$PWD/thirdparty/cubism
CUBISM_CORE_PATH = $$CUBISM_PATH/core
CUBISM_FRAMEWORK_PATH = $$CUBISM_PATH/framework

INCLUDEPATH += \
    $$CUBISM_FRAMEWORK_PATH \
    $$CUBISM_CORE_PATH/include \

HEADERS += \
    $$CUBISM_CORE_PATH/include/Live2DCubismCore.h \
    $$CUBISM_FRAMEWORK_PATH/CubismCdiJson.hpp \
    $$CUBISM_FRAMEWORK_PATH/CubismDefaultParameterId.hpp \
    $$CUBISM_FRAMEWORK_PATH/CubismFramework.hpp \
    $$CUBISM_FRAMEWORK_PATH/CubismFrameworkConfig.hpp \
    $$CUBISM_FRAMEWORK_PATH/CubismModelSettingJson.hpp \
    $$CUBISM_FRAMEWORK_PATH/Effect/CubismBreath.hpp \
    $$CUBISM_FRAMEWORK_PATH/Effect/CubismEyeBlink.hpp \
    $$CUBISM_FRAMEWORK_PATH/Effect/CubismPose.hpp \
    $$CUBISM_FRAMEWORK_PATH/ICubismAllocator.hpp \
    $$CUBISM_FRAMEWORK_PATH/ICubismModelSetting.hpp \
    $$CUBISM_FRAMEWORK_PATH/Id/CubismId.hpp \
    $$CUBISM_FRAMEWORK_PATH/Id/CubismIdManager.hpp \
    $$CUBISM_FRAMEWORK_PATH/Live2DCubismCore.hpp \
    $$CUBISM_FRAMEWORK_PATH/Math/CubismMath.hpp \
    $$CUBISM_FRAMEWORK_PATH/Math/CubismMatrix44.hpp \
    $$CUBISM_FRAMEWORK_PATH/Math/CubismModelMatrix.hpp \
    $$CUBISM_FRAMEWORK_PATH/Math/CubismTargetPoint.hpp \
    $$CUBISM_FRAMEWORK_PATH/Math/CubismVector2.hpp \
    $$CUBISM_FRAMEWORK_PATH/Math/CubismViewMatrix.hpp \
    $$CUBISM_FRAMEWORK_PATH/Model/CubismMoc.hpp \
    $$CUBISM_FRAMEWORK_PATH/Model/CubismModel.hpp \
    $$CUBISM_FRAMEWORK_PATH/Model/CubismModelUserData.hpp \
    $$CUBISM_FRAMEWORK_PATH/Model/CubismModelUserDataJson.hpp \
    $$CUBISM_FRAMEWORK_PATH/Model/CubismUserModel.hpp \
    $$CUBISM_FRAMEWORK_PATH/Motion/ACubismMotion.hpp \
    $$CUBISM_FRAMEWORK_PATH/Motion/CubismExpressionMotion.hpp \
    $$CUBISM_FRAMEWORK_PATH/Motion/CubismMotion.hpp \
    $$CUBISM_FRAMEWORK_PATH/Motion/CubismMotionInternal.hpp \
    $$CUBISM_FRAMEWORK_PATH/Motion/CubismMotionJson.hpp \
    $$CUBISM_FRAMEWORK_PATH/Motion/CubismMotionManager.hpp \
    $$CUBISM_FRAMEWORK_PATH/Motion/CubismMotionQueueEntry.hpp \
    $$CUBISM_FRAMEWORK_PATH/Motion/CubismMotionQueueManager.hpp \
    $$CUBISM_FRAMEWORK_PATH/Physics/CubismPhysics.hpp \
    $$CUBISM_FRAMEWORK_PATH/Physics/CubismPhysicsInternal.hpp \
    $$CUBISM_FRAMEWORK_PATH/Physics/CubismPhysicsJson.hpp \
    $$CUBISM_FRAMEWORK_PATH/Rendering/CubismRenderer.hpp \
    $$CUBISM_FRAMEWORK_PATH/Rendering/OpenGL/CubismOffscreenSurface_OpenGLES2.hpp \
    $$CUBISM_FRAMEWORK_PATH/Rendering/OpenGL/CubismRenderer_OpenGLES2.hpp \
    $$CUBISM_FRAMEWORK_PATH/Type/CubismBasicType.hpp \
    $$CUBISM_FRAMEWORK_PATH/Type/csmMap.hpp \
    $$CUBISM_FRAMEWORK_PATH/Type/csmRectF.hpp \
    $$CUBISM_FRAMEWORK_PATH/Type/csmString.hpp \
    $$CUBISM_FRAMEWORK_PATH/Type/csmVector.hpp \
    $$CUBISM_FRAMEWORK_PATH/Utils/CubismDebug.hpp \
    $$CUBISM_FRAMEWORK_PATH/Utils/CubismJson.hpp \
    $$CUBISM_FRAMEWORK_PATH/Utils/CubismString.hpp \

SOURCES += \
    $$CUBISM_FRAMEWORK_PATH/CubismCdiJson.cpp \
    $$CUBISM_FRAMEWORK_PATH/CubismDefaultParameterId.cpp \
    $$CUBISM_FRAMEWORK_PATH/CubismFramework.cpp \
    $$CUBISM_FRAMEWORK_PATH/CubismModelSettingJson.cpp \
    $$CUBISM_FRAMEWORK_PATH/Effect/CubismBreath.cpp \
    $$CUBISM_FRAMEWORK_PATH/Effect/CubismEyeBlink.cpp \
    $$CUBISM_FRAMEWORK_PATH/Effect/CubismPose.cpp \
    $$CUBISM_FRAMEWORK_PATH/Id/CubismId.cpp \
    $$CUBISM_FRAMEWORK_PATH/Id/CubismIdManager.cpp \
    $$CUBISM_FRAMEWORK_PATH/Math/CubismMath.cpp \
    $$CUBISM_FRAMEWORK_PATH/Math/CubismMatrix44.cpp \
    $$CUBISM_FRAMEWORK_PATH/Math/CubismModelMatrix.cpp \
    $$CUBISM_FRAMEWORK_PATH/Math/CubismTargetPoint.cpp \
    $$CUBISM_FRAMEWORK_PATH/Math/CubismVector2.cpp \
    $$CUBISM_FRAMEWORK_PATH/Math/CubismViewMatrix.cpp \
    $$CUBISM_FRAMEWORK_PATH/Model/CubismMoc.cpp \
    $$CUBISM_FRAMEWORK_PATH/Model/CubismModel.cpp \
    $$CUBISM_FRAMEWORK_PATH/Model/CubismModelUserData.cpp \
    $$CUBISM_FRAMEWORK_PATH/Model/CubismModelUserDataJson.cpp \
    $$CUBISM_FRAMEWORK_PATH/Model/CubismUserModel.cpp \
    $$CUBISM_FRAMEWORK_PATH/Motion/ACubismMotion.cpp \
    $$CUBISM_FRAMEWORK_PATH/Motion/CubismExpressionMotion.cpp \
    $$CUBISM_FRAMEWORK_PATH/Motion/CubismMotion.cpp \
    $$CUBISM_FRAMEWORK_PATH/Motion/CubismMotionJson.cpp \
    $$CUBISM_FRAMEWORK_PATH/Motion/CubismMotionManager.cpp \
    $$CUBISM_FRAMEWORK_PATH/Motion/CubismMotionQueueEntry.cpp \
    $$CUBISM_FRAMEWORK_PATH/Motion/CubismMotionQueueManager.cpp \
    $$CUBISM_FRAMEWORK_PATH/Physics/CubismPhysics.cpp \
    $$CUBISM_FRAMEWORK_PATH/Physics/CubismPhysicsJson.cpp \
    $$CUBISM_FRAMEWORK_PATH/Rendering/CubismRenderer.cpp \
    $$CUBISM_FRAMEWORK_PATH/Rendering/OpenGL/CubismOffscreenSurface_OpenGLES2.cpp \
    $$CUBISM_FRAMEWORK_PATH/Rendering/OpenGL/CubismRenderer_OpenGLES2.cpp \
    $$CUBISM_FRAMEWORK_PATH/Type/csmRectF.cpp \
    $$CUBISM_FRAMEWORK_PATH/Type/csmString.cpp \
    $$CUBISM_FRAMEWORK_PATH/Utils/CubismDebug.cpp \
    $$CUBISM_FRAMEWORK_PATH/Utils/CubismJson.cpp \
    $$CUBISM_FRAMEWORK_PATH/Utils/CubismString.cpp \

win32:CONFIG(release, debug|release): LIBS += -L$$CUBISM_CORE_PATH/lib/windows/ -lLive2DCubismCore_MD
else:win32:CONFIG(debug, debug|release): LIBS += -L$$CUBISM_CORE_PATH/lib/windows/ -lLive2DCubismCore_MDd
unix:!macx: LIBS +=  -L$$CUBISM_CORE_PATH/lib/linux/ -lLive2DCubismCore

# Stb image
STB_PATH = $$PWD/thirdparty/stb
INCLUDEPATH += \
    $$STB_PATH \

# Hook
win32: LIBS += -L$$OUT_PWD/../bin/lib/ -lHook
unix:!macx: LIBS += -L$$OUT_PWD/../bin/lib/ -lHook
INCLUDEPATH += $$PWD/../Hook
DEPENDPATH += $$PWD/../Hook

RESOURCES += \
    resources/resources.qrc

RC_ICONS = Core.ico

DESTDIR = ../bin
