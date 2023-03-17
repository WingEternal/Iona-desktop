#ifndef IONA_DESKTOP_CORE_DISPLAY_L2D_H
#define IONA_DESKTOP_CORE_DISPLAY_L2D_H

#include "gl_object_base.h"
#include "l2d_utils.h"
#include "l2d_model.h"
#include "l2d_touch_manager.h"

#include "Math/CubismMatrix44.hpp"
#include "Math/CubismViewMatrix.hpp"
#include "CubismFramework.hpp"
#include "Rendering/OpenGL/CubismOffscreenSurface_OpenGLES2.hpp"

#include <QPoint>

namespace IonaDesktop {
namespace CoreDisplay {
    class GLObj_L2d : public CoreDisplay::GLObjectBase
    {
        Q_OBJECT
    public:
        explicit GLObj_L2d(QOpenGLWidget* parent, const QMatrix4x4& tf_camera_);
        ~GLObj_L2d();

        virtual void init() final;
        virtual void paint() final;
        void setModelPath(const Csm::csmChar* path, const Csm::csmChar* fileName);

    public slots:
        void onL2dLButtonDown(const QPointF& pt);
        void onL2dMouseMove(const QPointF& pt);
        void onL2dLButtonUp(const QPointF& pt);

    private:
        L2dModel* _model;

        static void initializeCubism();
        static L2dAllocator _cubismAllocator;
        static Csm::CubismFramework::Option _cubismOption;
        bool _flag_path_ready;
        Csm::csmString _modelHomeDir;
        Csm::csmString _modelFileName;

        void initializeShaders();
        QOpenGLShaderProgram *sprogram_general;
        // use to set uniform value for shader
        int attr_sp_general_Transform;
        int attr_sp_general_Texture_0;

        void initializeRenderPlane();
        QOpenGLVertexArrayObject *vao_plane;
        GLfloat vertices[20];

        QVector3D base_position;
        const QMatrix4x4& tf_camera;
        int _canvas_width;
        int _canvas_height;
        void PreModelDraw();
        void ModelDraw();
        void PostModelDraw();

        QPointF scrPosSigmoid(const double virt_x, const double virt_y) const;
        float TransformViewX(float deviceX) const;
        float TransformViewY(float deviceY) const;
        float TransformScreenX(float deviceX) const;
        float TransformScreenY(float deviceY) const;

        L2dTouchManager* _touchManager;                 ///< タッチマネージャー
        Csm::CubismMatrix44* _deviceToScreen;    ///< デバイスからスクリーンへの行列
        Csm::CubismViewMatrix* _viewMatrix;      ///< viewMatrix
        // virtual screen geometry
        QRect virtscr_geometry;
        bool flag_mouse_pressed;

        Csm::Rendering::CubismOffscreenFrame_OpenGLES2 _renderBuffer;   ///< モードによってはCubismモデル結果をこっちにレンダリング
        float _clearColor[4];           ///< レンダリングターゲットのクリアカラー
    };
}
}

#endif // IONA_DESKTOP_CORE_DISPLAY_L2D_H
