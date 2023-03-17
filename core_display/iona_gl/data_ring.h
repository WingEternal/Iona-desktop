#ifndef IONA_DESKTOP_CORE_DISPLAY_DATA_RING_H
#define IONA_DESKTOP_CORE_DISPLAY_DATA_RING_H

#include "gl_object_base.h"
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <memory>

using namespace IonaDesktop;

// Forward Declaration
class QSignalMapper;

namespace IonaDesktop {
namespace CoreDisplay {
    class GLObj_DataRing  : public GLObjectBase
    {
        Q_OBJECT
        Q_PROPERTY(float RingScale READ RingScale WRITE setRingScale)
        Q_PROPERTY(float RingRpm READ RingRpm WRITE setRingRpm)
        Q_PROPERTY(QVector3D RingTPos READ RingTPos WRITE setRingTPos)
        Q_PROPERTY(QVector3D RingMPos READ RingMPos WRITE setRingMPos)
        Q_PROPERTY(QVector3D RingBPos READ RingBPos WRITE setRingBPos)

    public:
        explicit GLObj_DataRing(QOpenGLWidget* parent, const QMatrix4x4& tf_camera_);
        ~GLObj_DataRing() override;

        virtual void init()  final;
        virtual void paint() final;

        // Params
        // Ring radius
        static constexpr GLfloat ring_tb_height = 5.0f;
        static constexpr GLfloat ring_tbf_height = 5.2f; // tb_frame
        static constexpr GLfloat ring_m_height = 6.0f;
        // Ring resolution (N face per ring)
        static constexpr GLint ring_tb_res = 52;
        static constexpr GLint ring_tbf_res = 52;
        static constexpr GLint ring_m_res = 52;

        // Property - Getter
        float RingScale();
        float RingRpm();
        QVector3D RingTPos();
        QVector3D RingMPos();
        QVector3D RingBPos();

    signals:
        void taskFinished(const QString task_name);

    public slots:
        // Property - Setter
        void setRingScale(const float scale);
        void setRingRpm(const float rpm);
        void setRingTPos(const QVector3D& pos);
        void setRingMPos(const QVector3D& pos);
        void setRingBPos(const QVector3D& pos);

        void addTask(const QString& task_name, const QVariant& target_state, const double interval, const QEasingCurve::Type curve = QEasingCurve::Linear);

    private:
        // Generate model at compile time
        constexpr void CompileVertices
            (GLfloat *container_array, const GLint size,
             const GLfloat radius, const GLfloat height, const GLint res);

        // Shader
        void initializeShaders();
        void loadGenernalShader();
        QOpenGLShaderProgram* sprogram_general;
        // use to set uniform value for shader
        int attr_sp_general_Transform;
        int attr_sp_general_Texture_0;
        int attr_sp_general_Delta_Tex_S;
        void loadMix2Shader();
        QOpenGLShaderProgram* sprogram_mix2;
        int attr_sp_mix2_Transform;
        int attr_sp_mix2_Texture_0;
        int attr_sp_mix2_Texture_1;
        int attr_sp_mix2_Delta0f_Tex_S;
        int attr_sp_mix2_Delta1f_Tex_T;
        void loadRmShader();
        QOpenGLShaderProgram* sprogram_rm;
        int attr_sp_rm_Transform;
        int attr_sp_rm_Texture_0;
        int attr_sp_rm_Texture_1;
        int attr_sp_rm_Delta_Tex_S;
        int attr_sp_rm_Active_Zone;

        // Texture
        void initializeTexture();
        QOpenGLTexture* loadTexture(const QString &address);
        QOpenGLTexture* tex_ring_tb[3]; // [0] = frame, [1] = block, [2] = code
        QOpenGLTexture* tex_ring_m_empty;
        QOpenGLTexture* tex_ring_m_active[20];
        int animate_switch_thrd;
        int animate_switch_counter;
        int ring_m_active_zone;
        int ring_m_animate_counter;

        // Model (Vertices,  VAO, VBO)
        void initializeModel();
        QOpenGLVertexArrayObject* loadModel
            (QOpenGLShaderProgram* sp, const GLfloat* vertices, const size_t vertices_size);
        QOpenGLVertexArrayObject* vao_ring_tb;
        QOpenGLVertexArrayObject* vao_ring_tbf;
        QOpenGLVertexArrayObject* vao_ring_m;

        // Paint
        QVector3D base_position;
        const QMatrix4x4& tf_camera;
        void drawRingTB();
        void drawRingTBF();
        void drawRingM();

        // Position bias
        QVector3D _ring_t_posb;
        QVector3D _ring_m_posb;
        QVector3D _ring_b_posb;

        // Spin && roll
        float _ring_rpm;
        float ring_spin_angle; // indicate normalized ring rotation angle [0, 1)
        float ring_tb_roll_spd;
        float ring_tb_roll_dist[2];
        float random_stop_elapsed[2];

        // Scale
        float _ring_scale;

        // Animation (Transit)
        static constexpr int _drqpa_count = 5;
        /**
         * 0: RingScale
         * 1: RingRpm
         * 2: RingTPos
         * 3: RingMPos
         * 4: RingBPos
        */
        QPropertyAnimation* _qpa[_drqpa_count];
        template<typename T>
        void addTaskPrivate(QPropertyAnimation* _qpa, const T& start_state, const QVariant& end_state, const double interval, const QEasingCurve::Type curve)
        {
            if(_qpa->state() == QPropertyAnimation::Running)
                _qpa->pause();
            _qpa->setStartValue(start_state);
            _qpa->setEndValue(end_state);
            _qpa->setDuration(interval);
            _qpa->setEasingCurve(curve);
            _qpa->start();
        }
        void execStartMotion();
    };
}
}
#endif // IONA_DESKTOP_CORE_DISPLAY_DATA_RING_H
