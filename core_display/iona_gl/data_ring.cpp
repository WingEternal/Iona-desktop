#include "iona_gl/data_ring.h"
#include "app/app_config.h"
#include <QTime>

using namespace IonaDesktop::CoreDisplay;
using IonaDesktop::CoreDisplay::GLHandle;

namespace {
    static constexpr const char* vs_general_src  =
        "attribute vec4 Pos4f; \
        attribute vec2 TexCoord2f_0; \
        uniform mat4 Transform; \
        varying vec2 Tex2f_0; \
        void main(void) \
        { \
            gl_Position = Transform * Pos4f; \
            Tex2f_0 = TexCoord2f_0; \
        }";
    static constexpr const char* fs_general_src =
        "uniform sampler2D Texture_0; \
        uniform float Delta_Tex_S; \
        varying vec2 Tex2f_0; \
        void main(void) \
        { \
            gl_FragColor = texture2D(Texture_0, Tex2f_0.st + vec2(Delta_Tex_S, 0.0)); \
        }";
    /*
     * fragment_shader_mix2:
     * formula from [fifbro](https://blog.csdn.net/qq_21950929/article/details/78989215)
    */
    static constexpr const char* fs_mix2_src =
        "uniform sampler2D Texture_0; \
        uniform sampler2D Texture_1; \
        uniform float Delta0f_Tex_S; \
        uniform float Delta1f_Tex_T; \
        varying vec2 Tex2f_0; \
        void main(void) \
        { \
            vec4 texture_0_color = texture2D(Texture_0, Tex2f_0.st + vec2(Delta0f_Tex_S, 0.0)); \
            vec4 texture_1_color = texture2D(Texture_1, vec2(Tex2f_0.s + Delta0f_Tex_S, Tex2f_0.t / 6.0 + Delta1f_Tex_T)); \
            float mix_alpha = texture_0_color.a + texture_1_color.a - texture_0_color.a * texture_1_color.a; \
            vec3 mix_rgb = (texture_0_color.a * (1.0 - texture_1_color.a) * texture_0_color.rgb + texture_1_color.a * texture_1_color.rgb) / mix_alpha; \
            gl_FragColor = vec4(mix_rgb, mix_alpha); \
        }";
    static constexpr const char* vs_rm_src =
        "attribute vec4 Pos4f; \
        attribute vec2 TexCoord2f_0; \
        uniform mat4 Transform; \
        uniform int Active_Zone; \
        uniform float Delta_Tex_S; \
        varying vec2 Tex2f_Active; \
        varying vec2 Tex2f_Empty; \
        void main(void) \
        { \
            gl_Position = Transform * Pos4f; \
            Tex2f_Active = vec2(TexCoord2f_0.s + Delta_Tex_S + float(Active_Zone) / 4.0, TexCoord2f_0.t); \
            Tex2f_Empty = vec2((TexCoord2f_0.s + Delta_Tex_S) * 4.0, TexCoord2f_0.t); \
        }";
    static constexpr const char* fs_rm_src =
        "uniform sampler2D Texture_0; \
        uniform sampler2D Texture_1; \
        varying vec2 Tex2f_Active; \
        varying vec2 Tex2f_Empty; \
        void main(void) \
        { \
            vec4 texture_0_color = texture2D(Texture_0, Tex2f_Active.st); \
            if(texture_0_color.a != 0) { gl_FragColor = texture_0_color; } \
            else { gl_FragColor = texture2D(Texture_1, Tex2f_Empty.st); } \
        }";

    float genRandomDouble(const float min, const float max)
    {
        static bool seedStatus = false;
        if (!seedStatus)
        {
            srand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
            seedStatus = true;
        }
        float diff = fabs(max - min);
        float m1 = static_cast<float>(rand() % 100) / 100.0f;
        return min + m1 * diff;
    }
}

GLObj_DataRing::GLObj_DataRing(QOpenGLWidget* parent, const QMatrix4x4& tf_camera_)
    : GLObjectBase(parent),
      tex_ring_tb{nullptr},
      tex_ring_m_empty(nullptr),
      tex_ring_m_active{nullptr},
      animate_switch_counter(0),
      ring_m_active_zone(0),
      ring_m_animate_counter(0),
      base_position(0, 0, -75.0),
      tf_camera(tf_camera_),
      ring_spin_angle(0),
      ring_tb_roll_dist{0.0f, 0.0f},
      random_stop_elapsed{-1.0f, -1.0f}
{
    AppConfig::getInstance().getParam("/animate/ring_spin_rpm", ring_spin_rpm);
    AppConfig::getInstance().getParam("/animate/ring_m_animation_switch_by_frame", animate_switch_thrd);
    AppConfig::getInstance().getParam("/animate/ring_tb_roll_spd", ring_tb_roll_spd);
}

GLObj_DataRing::~GLObj_DataRing()
{
    for(int i = 0; i < 3; i++)
        tex_ring_tb[i]->destroy();
    tex_ring_m_empty->destroy();
    for(int i = 0; i < 20; i++)
        tex_ring_m_active[i]->destroy();
}

void GLObj_DataRing::init()
{
    initializeTexture();
    initializeShaders();
    initializeModel();
}

QOpenGLTexture* GLObj_DataRing::loadTexture(const QString &address)
{
    QOpenGLTexture* tex_ptr = new QOpenGLTexture(QImage(address).mirrored(), QOpenGLTexture::GenerateMipMaps);
    tex_ptr->setMinificationFilter(QOpenGLTexture::Linear);
    tex_ptr->setMagnificationFilter(QOpenGLTexture::Linear);
    tex_ptr->setWrapMode(QOpenGLTexture::Repeat);
    return tex_ptr;
}

void GLObj_DataRing::initializeTexture()
{
    tex_ring_tb[0] = loadTexture(":/charater/texture/ring_tb_frame_2648_75.png");
    tex_ring_tb[1] = loadTexture(":/charater/texture/ring_tb_block_2648_75.png");
    tex_ring_tb[2] = loadTexture(":/charater/texture/ring_tb_code_2648.png");
    tex_ring_m_empty = loadTexture(":/charater/texture/ring_m_empty_559.png");
    for(int i = 0; i < 20; i++)
        tex_ring_m_active[i] = loadTexture(":/charater/texture/ring_m_a"+ QString::number(i) + "_559.png");
}

void GLObj_DataRing::initializeShaders()
{
    loadGenernalShader();
    loadMix2Shader();
    loadRmShader();
}

void GLObj_DataRing::loadGenernalShader()
{
    // sp_general for portrait
    sprogram_general = new QOpenGLShaderProgram(this);
    sprogram_general->addShaderFromSourceCode(QOpenGLShader::Vertex, vs_general_src);
    sprogram_general->addShaderFromSourceCode(QOpenGLShader::Fragment, fs_general_src);
    if(!sprogram_general->link()) {
        qDebug() << "General Shader Program Link Failed";
    }
    attr_sp_general_Transform = sprogram_general->uniformLocation("Transform");
    attr_sp_general_Texture_0 = sprogram_general->uniformLocation("Texture_0");
    attr_sp_general_Delta_Tex_S = sprogram_general->uniformLocation("Delta_Tex_S");
}

void GLObj_DataRing::loadMix2Shader()
{
    // sp_mix2 for ring_tb
    sprogram_mix2 = new QOpenGLShaderProgram(this);
    sprogram_mix2->addShaderFromSourceCode(QOpenGLShader::Vertex, vs_general_src);
    sprogram_mix2->addShaderFromSourceCode(QOpenGLShader::Fragment, fs_mix2_src);
    if(!sprogram_mix2->link()) {
        qDebug() << "Mix2 Shader Program Link Failed";
    }
    attr_sp_mix2_Transform = sprogram_mix2->uniformLocation("Transform");
    attr_sp_mix2_Texture_0 = sprogram_mix2->uniformLocation("Texture_0");
    attr_sp_mix2_Texture_1 = sprogram_mix2->uniformLocation("Texture_1");
    attr_sp_mix2_Delta0f_Tex_S = sprogram_mix2->uniformLocation("Delta0f_Tex_S");
    attr_sp_mix2_Delta1f_Tex_T = sprogram_mix2->uniformLocation("Delta1f_Tex_T");
}

void GLObj_DataRing::loadRmShader()
{
    // sp_rm for ring_m
    sprogram_rm = new QOpenGLShaderProgram(this);
    sprogram_rm->addShaderFromSourceCode(QOpenGLShader::Vertex, vs_rm_src);
    sprogram_rm->addShaderFromSourceCode(QOpenGLShader::Fragment, fs_rm_src);
    if(!sprogram_rm->link())
        qDebug() << "Rm Shader Program Link Failed";
    attr_sp_rm_Transform = sprogram_rm->uniformLocation("Transform");
    attr_sp_rm_Texture_0 = sprogram_rm->uniformLocation("Texture_0");
    attr_sp_rm_Texture_1 = sprogram_rm->uniformLocation("Texture_1");
    attr_sp_rm_Delta_Tex_S = sprogram_rm->uniformLocation("Delta_Tex_S");
    attr_sp_rm_Active_Zone = sprogram_rm->uniformLocation("Active_Zone");
}

void GLObj_DataRing::paint()
{
    glAlphaFunc(GL_GREATER, 0.1f);
    glEnable(GL_ALPHA_TEST);
    // Calculate spin angle
    auto elapsed_time = L2dPal::GetDeltaTime();
    ring_spin_angle += ring_spin_rpm * elapsed_time / 60.0f;
    if(ring_spin_angle > 1)
        ring_spin_angle -= 1;

    // Update roll dist
    for(int i = 0; i < 2; i++)
    {
        if(random_stop_elapsed[i] <= 0)
        {
            ring_tb_roll_dist[i] += ring_tb_roll_spd * elapsed_time / 60.0f;
            if(ring_tb_roll_dist[i] > 1)
                ring_tb_roll_dist[i] -= 1;
            if(genRandomDouble(0.0, 1.0) > 0.95f)
                random_stop_elapsed[i] = genRandomDouble(0.2f, 0.5f);
        }
        else
            random_stop_elapsed[i] -= elapsed_time;
    }

    drawRingTB();
    drawRingTBF();
    drawRingM();
    glDisable(GL_ALPHA_TEST);
}

void GLObj_DataRing::drawRingTB()
{
    sprogram_mix2->bind();
    sprogram_mix2->setUniformValue(attr_sp_mix2_Delta0f_Tex_S, - ring_spin_angle);
    sprogram_mix2->setUniformValue(attr_sp_mix2_Delta1f_Tex_T, - ring_tb_roll_dist[0]);
    vao_ring_tb->bind();
    GLHandle::get()->glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    tex_ring_tb[1]->bind();
    GLHandle::get()->glActiveTexture(GL_TEXTURE1);
    glEnable(GL_TEXTURE_2D);
    tex_ring_tb[2]->bind();
    sprogram_mix2->setUniformValue(attr_sp_mix2_Texture_0, 0);
    sprogram_mix2->setUniformValue(attr_sp_mix2_Texture_1, 1);
    QMatrix4x4 tf_ring_tb;

    // Draw ring_t
    tf_ring_tb.setToIdentity();
    tf_ring_tb.translate(base_position.x() + 0.0f, base_position.y() + 20.0f, base_position.z());
    tf_ring_tb.rotate(10, 1, 0, 0);
    sprogram_mix2->setUniformValue(attr_sp_mix2_Transform, tf_camera * tf_ring_tb);
    glDrawArrays(GL_QUAD_STRIP, 0, 2 * (ring_tb_res + 1));

    // Draw ring_b
    sprogram_mix2->setUniformValue(attr_sp_mix2_Delta0f_Tex_S, - ring_spin_angle + 0.5f);
    sprogram_mix2->setUniformValue(attr_sp_mix2_Delta1f_Tex_T, - ring_tb_roll_dist[1] + 0.5f);
    tf_ring_tb.setToIdentity();
    tf_ring_tb.translate(base_position.x() + 0.0f, base_position.y() - 8.0f, base_position.z());
    tf_ring_tb.rotate(5, 1, 0, 0);
    sprogram_mix2->setUniformValue(attr_sp_mix2_Transform, tf_camera * tf_ring_tb);
    glDrawArrays(GL_QUAD_STRIP, 0, 2 * (ring_tb_res + 1));

    // Clean
    glDisable(GL_TEXTURE_2D);
    tex_ring_tb[1]->release();
    tex_ring_tb[2]->release();
    vao_ring_tb->release();
    sprogram_mix2->release();
}

void GLObj_DataRing::drawRingTBF()
{
    // Draw ring_tbf
    sprogram_general->bind();
    vao_ring_tbf->bind();
    GLHandle::get()->glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    tex_ring_tb[0]->bind();
    sprogram_general->setUniformValue(attr_sp_general_Texture_0, 0);
    QMatrix4x4 tf_ring_tbf;
    tf_ring_tbf.setToIdentity();
    tf_ring_tbf.translate(base_position.x() + 0.0f, base_position.y() + 20.0f, base_position.z());
    tf_ring_tbf.rotate(10, 1, 0, 0);
    sprogram_general->setUniformValue(attr_sp_general_Transform, tf_camera * tf_ring_tbf);
    glDrawArrays(GL_QUAD_STRIP, 0, 2 * (ring_tbf_res + 1));
    tf_ring_tbf.setToIdentity();
    tf_ring_tbf.translate(base_position.x() + 0.0f, base_position.y() - 8.0f, base_position.z());
    tf_ring_tbf.rotate(5, 1, 0, 0);
    sprogram_general->setUniformValue(attr_sp_general_Transform, tf_camera * tf_ring_tbf);
    glDrawArrays(GL_QUAD_STRIP, 0, 2 * (ring_tbf_res + 1));

    // Clean
    glDisable(GL_TEXTURE_2D);
    tex_ring_tb[0]->release();
    vao_ring_m->release();
    sprogram_general->release();
}

void GLObj_DataRing::drawRingM()
{
    // Draw ring_m
    sprogram_rm->bind();
    vao_ring_m->bind();
    GLHandle::get()->glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    tex_ring_m_active[ring_m_animate_counter]->bind();
    GLHandle::get()->glActiveTexture(GL_TEXTURE1);
    glEnable(GL_TEXTURE_2D);
    tex_ring_m_empty->bind();
    sprogram_rm->setUniformValue(attr_sp_rm_Texture_0, 0);
    sprogram_rm->setUniformValue(attr_sp_rm_Texture_1, 1);
    sprogram_rm->setUniformValue(attr_sp_rm_Delta_Tex_S, ring_spin_angle);
    sprogram_rm->setUniformValue(attr_sp_rm_Active_Zone, ring_m_active_zone);
    QMatrix4x4 tf_ring_m;
    tf_ring_m.setToIdentity();
    tf_ring_m.translate(base_position.x() + 0.0f,  base_position.y() + 5.0f, base_position.z());
    tf_ring_m.rotate(5, 1, 0, 0);
    sprogram_rm->setUniformValue(attr_sp_rm_Transform, tf_camera * tf_ring_m);
    glDrawArrays(GL_QUAD_STRIP, 0, 2 * (ring_m_res + 1));
    // Clean
    glDisable(GL_TEXTURE_2D);
    tex_ring_m_empty->release();
    tex_ring_m_active[ring_m_animate_counter]->release();
    vao_ring_m->release();
    sprogram_rm->release();

    animate_switch_counter++;
    if(animate_switch_counter >= animate_switch_thrd)
    {
        animate_switch_counter = 0;
        ring_m_animate_counter++;
        if(ring_m_animate_counter >= 20)
        {
            ring_m_animate_counter = 0;
            ring_m_active_zone++;
            if(ring_m_active_zone >= 4)
                ring_m_active_zone = 0;
        }
    }
}
