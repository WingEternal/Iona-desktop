# Cubism
QT  += opengl

CUBISM_PATH = $$PWD
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
