#ifndef IONA_DESKTOP_CORE_DISPLAY_L2D_MODEL_H
#define IONA_DESKTOP_CORE_DISPLAY_L2D_MODEL_H

#include <CubismFramework.hpp>
#include <Model/CubismUserModel.hpp>
#include <ICubismModelSetting.hpp>
#include <Type/csmRectF.hpp>
#include <Rendering/OpenGL/CubismOffscreenSurface_OpenGLES2.hpp>

#include "l2d_tex_manager.h"

namespace IonaDesktop {
namespace CoreDisplay {
    class L2dModel : public Csm::CubismUserModel
    {
    public:
        explicit L2dModel();
        ~L2dModel();

        /* Initialize */
    public:
        void LoadAssets(const Csm::csmChar* dir, const  Csm::csmChar* fileName);
    private:
        Csm::csmString _modelHomeDir;

        Csm::ICubismModelSetting* _modelSetting;
        void SetupModel(Csm::ICubismModelSetting* setting);
        void SetupTextures();
        L2dTexManager* _texManager;

        /* Paint */
    public:
        void ReloadRenderer();
        void Update();
        void Draw(Csm::CubismMatrix44& matrix);

        /* Motion && Expression */
    public:
        Csm::CubismMotionQueueEntryHandle StartMotion
            (const Csm::csmChar* group, Csm::csmInt32 no, Csm::csmInt32 priority,
             Csm::ACubismMotion::FinishedMotionCallback onFinishedMotionHandler = nullptr);
        Csm::CubismMotionQueueEntryHandle StartRandomMotion
            (const Csm::csmChar* group, Csm::csmInt32 priority,
             Csm::ACubismMotion::FinishedMotionCallback onFinishedMotionHandler = nullptr);

        void SetExpression(const Csm::csmChar* expressionID);
        void SetRandomExpression();

    private:
        void PreloadMotionGroup(const Csm::csmChar* group);
        void ReleaseMotionGroup(const Csm::csmChar* group) const;
        void ReleaseMotions();
        void ReleaseExpressions();

        Csm::csmVector<Csm::CubismIdHandle> _eyeBlinkIds;
        Csm::csmVector<Csm::CubismIdHandle> _lipSyncIds;
        Csm::csmMap<Csm::csmString, Csm::ACubismMotion*>   _motions;
        Csm::csmMap<Csm::csmString, Csm::ACubismMotion*>   _expressions;

        /* Event && HitBox */
    public:
        virtual void MotionEventFired
            (const Live2D::Cubism::Framework::csmString& eventValue);
        virtual Csm::csmBool HitTest
            (const Csm::csmChar* hitAreaName, Csm::csmFloat32 x, Csm::csmFloat32 y);
    private:
        Csm::csmVector<Csm::csmRectF> _hitArea;
        Csm::csmVector<Csm::csmRectF> _userArea;
        const Csm::CubismId* _idParamAngleX;
        const Csm::CubismId* _idParamAngleY;
        const Csm::CubismId* _idParamAngleZ;
        const Csm::CubismId* _idParamBodyAngleX;
        const Csm::CubismId* _idParamEyeBallX;
        const Csm::CubismId* _idParamEyeBallY;

        /* Utils */
    private:
        Csm::csmFloat32 _userTimeSeconds;
    };
}
}

#endif // IONA_DESKTOP_CORE_DISPLAY_L2D_MODEL_H
