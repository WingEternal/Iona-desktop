#ifndef IONADESKTOP_CORE_GL_WIDGET_H
#define IONADESKTOP_CORE_GL_WIDGET_H
/* GLOBAL SETTINGS */
#include "core/core_global.h"
#include <QTimer>
/* GL INCLUDES */
#include <QWidget>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <GL/glu.h>
#include <QPixmap>
#include "core/gl_widget_model.h"

namespace IonaDesktop {
namespace Core {
    class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
    {
        Q_OBJECT
        /* Widget Initialization */
    public:
        explicit GLWidget(const QRect& geo, QWidget *parent = nullptr);
        ~GLWidget() override;
    private:
        QRect widget_geo;
        /* GL Methods */
    protected:
        void initializeGL() override;
    private:
        // Shader
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
        void loadTexture(QOpenGLTexture **tex_ptr2, const QString &address);
        QOpenGLTexture *tex_portrait;
        QOpenGLTexture *tex_ring_tb[2]; // [0] = block, [1] = frame
        QOpenGLTexture *tex_ring_m;

        // Model (Vertices,  VAO, VBO)
    private:
        void initializeModel();
        void loadModel(QOpenGLVertexArrayObject *vao, QOpenGLShaderProgram *sp, const GLfloat *vertices, const size_t vertices_size);
//        void loadRingTBModel();
//        void loadRingMModel();
        QOpenGLVertexArrayObject *vao_portrait;
        QOpenGLVertexArrayObject *vao_ring_tb;
        QOpenGLVertexArrayObject *vao_ring_m;

        // Camera
    private:
        QMatrix4x4  tf_camera;
        // Describe the depth (z) of portrait and rings
        const QVector3D iona_position = QVector3D(0, 0, -75.0);
        // Paint
    protected:
        void resizeGL(int w, int h) override;
        void paintGL() override;
    private:
        void drawPortrait();
        void drawRingTB();
        void drawRingM();

        // MISC
    private:
        QTimer *ring_spin_update_timer;
        double ring_spin_angle;
    private slots:
        void Slot_RingSpinUpdateTimer_Timeout();
    };
}
}

#endif // IONADESKTOP_CORE_GL_WIDGET_H
