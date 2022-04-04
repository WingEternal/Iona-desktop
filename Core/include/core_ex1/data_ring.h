#ifndef IONA_DESKTOP_CORE_EX1_DATA_RING_H
#define IONA_DESKTOP_CORE_EX1_DATA_RING_H

#include "core/gl_entry.h"
#include "core/gl_object_base.h"
#include <QTimer>

using namespace IonaDesktop;

namespace IonaDesktop {
namespace CoreEx1 {
    class GLObj_DataRing  : public IonaDesktop::Core::GLObjectBase
    {
        Q_OBJECT
    public:
        explicit GLObj_DataRing(QOpenGLWidget* parent, const QMatrix4x4& tf_camera_);
        ~GLObj_DataRing();

        virtual void init()  final;
        virtual void paint() final;

        // Params
    public:
        // Ring radius
        static constexpr GLfloat ring_tb_height = 5.0f;
        static constexpr GLfloat ring_m_height = 6.0f;
        // Ring resolution (N face per ring)
        static constexpr GLint ring_tb_res = 50;
        static constexpr GLint ring_m_res = 50;
    private:
        // Generate model at compile time
        constexpr void CompileVertices
            (GLfloat *container_array, const GLint size,
             const GLfloat radius, const GLfloat height, const GLint res);

        // Shader
    private:
        void initializeShaders();
        void loadGenernalShader();
        void loadMix2Shader();
        QOpenGLShaderProgram *sprogram_general;
        // use to set uniform value for shader
        int attr_sp_general_Transform;
        int attr_sp_general_Texture_0;
        QOpenGLShaderProgram *sprogram_mix2;
        int attr_sp_mix2_Transform;
        int attr_sp_mix2_Texture_0;
        int attr_sp_mix2_Texture_1;
        int attr_sp_mix2_Delta_Tex_S;

        // Texture
    private:
        void initializeTexture();
        QOpenGLTexture * loadTexture(const QString &address);
        QOpenGLTexture *tex_ring_tb[2]; // [0] = block, [1] = frame
        QOpenGLTexture *tex_ring_m;

        // Model (Vertices,  VAO, VBO)
    private:
        void initializeModel();
        QOpenGLVertexArrayObject * loadModel
            (QOpenGLShaderProgram *sp, const GLfloat *vertices, const size_t vertices_size);
        QOpenGLVertexArrayObject *vao_ring_tb;
        QOpenGLVertexArrayObject *vao_ring_m;

        // Paint
    private:
        QVector3D base_position;
        const QMatrix4x4& tf_camera;
        void drawRingTB();
        void drawRingM();

        // Spin Timer
    private:
        QTimer *ring_spin_update_timer_ptr;
        double ring_spin_angle;
    private slots:
        void Slot_RingSpinUpdateTimer_Timeout();
    };
}
}
#endif // IONA_DESKTOP_CORE_EX1_DATA_RING_H
