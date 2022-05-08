#ifndef IONA_DESKTOP_CORE_EX1_L2D_H
#define IONA_DESKTOP_CORE_EX1_L2D_H

#include "core/gl_object_base.h"
#include "core_ex1/l2d_allocator.h"
#include "core_ex1/l2d_model.h"
#include "core_ex1/l2d_touch_manager.h"

#include "Math/CubismMatrix44.hpp"
#include "Math/CubismViewMatrix.hpp"
#include "CubismFramework.hpp"
#include "Rendering/OpenGL/CubismOffscreenSurface_OpenGLES2.hpp"

namespace IonaDesktop {
namespace CoreEx1 {
    class GLObj_L2d : public Core::GLObjectBase
    {
        Q_OBJECT
    public:
        explicit GLObj_L2d(QOpenGLWidget* parent, const QMatrix4x4& tf_camera_,  const QRect& canvas_size);
        ~GLObj_L2d();
    private:
        L2dModel* _model;

        /* Initialization */
    public:
        virtual void init() final;
    private:
        static void initializeCubism();
        static L2dAllocator _cubismAllocator;
        static Csm::CubismFramework::Option _cubismOption;

    public:
        void setModelPath(const Csm::csmChar* path, const Csm::csmChar* fileName);
    private:
        bool _flag_pathReady;
        Csm::csmString _modelHomeDir;
        Csm::csmString _modelFileName;

    private:
        void initializeShaders();
        QOpenGLShaderProgram *sprogram_general;
        // use to set uniform value for shader
        int attr_sp_general_Transform;
        int attr_sp_general_Texture_0;

    private:
        void initializeRenderPlane();
        QOpenGLVertexArrayObject *vao_plane;
        GLfloat vertices[20];

        /* Paint */
    public:
        virtual void paint() final;
    private:
        QVector3D base_position;
        const QMatrix4x4& tf_camera;
        int _canvas_width;
        int _canvas_height;
        void PreModelDraw();
        void ModelDraw();
        void PostModelDraw();

        /* Touches */
    public:
        void onTouchesBegan(QMouseEvent *e);
        void onTouchesMoved(QMouseEvent *e);
        void onTouchesEnd(QMouseEvent *e);
    private:
        float TransformViewX(float deviceX) const;
        float TransformViewY(float deviceY) const;
        float TransformScreenX(float deviceX) const;
        float TransformScreenY(float deviceY) const;

        L2dTouchManager* _touchManager;                 ///< タッチマネージャー
        Csm::CubismMatrix44* _deviceToScreen;    ///< デバイスからスクリーンへの行列
        Csm::CubismViewMatrix* _viewMatrix;      ///< viewMatrix
        QRect virtual_screen_geometry;

        /* Render */
    private:
        Csm::Rendering::CubismOffscreenFrame_OpenGLES2 _renderBuffer;   ///< モードによってはCubismモデル結果をこっちにレンダリング
        float _clearColor[4];           ///< レンダリングターゲットのクリアカラー
    };
}
}

#endif // IONA_DESKTOP_CORE_EX1_L2D_H
