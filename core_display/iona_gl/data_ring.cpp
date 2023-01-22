#include "data_ring.h"
#include "app_config.h"

using namespace IonaDesktop::CoreDisplay;
using IonaDesktop::CoreDisplay::GLEntry;

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
            uniform float Delta1f_Tex_S; \
            varying vec2 Tex2f_0; \
            void main(void) \
            { \
                vec4 texture_0_color = texture2D(Texture_0, Tex2f_0.st + vec2(Delta1f_Tex_S, 0.0)); \
                vec4 texture_1_color = texture2D(Texture_1, Tex2f_0.st); \
                float mix_alpha = texture_0_color.a + texture_1_color.a - texture_0_color.a * texture_1_color.a; \
                vec3 mix_rgb = (texture_0_color.a * (1.0 - texture_1_color.a) * texture_0_color.rgb + texture_1_color.a * texture_1_color.rgb) / mix_alpha; \
                gl_FragColor = vec4(mix_rgb, mix_alpha); \
            }";
}

GLObj_DataRing::GLObj_DataRing(QOpenGLWidget* parent, const QMatrix4x4& tf_camera_)
    : GLObjectBase(parent),
      attr_sp_general_Transform(-1),
      attr_sp_general_Texture_0(-1),
      attr_sp_general_Delta_Tex_S(-1),
      attr_sp_mix2_Transform(-1),
      attr_sp_mix2_Texture_0(-1),
      attr_sp_mix2_Texture_1(-1),
      attr_sp_mix2_Delta_Tex_S(-1),
      tex_ring_tb{nullptr, nullptr},
      tex_ring_m(nullptr),
      base_position(0, 0, -75.0),
      tf_camera(tf_camera_),
      ring_spin_angle(0)
{
    AppConfig::getInstance().getParam("/animate/ring_spin_rpm", ring_spin_rpm);
}
GLObj_DataRing::~GLObj_DataRing()
{
    for(int i = 0; i < 2; i++)
        tex_ring_tb[i]->destroy();
    tex_ring_m->destroy();
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
    tex_ring_tb[0] = loadTexture(":/charater/texture/ring_tb_block_2648_75.png");
    tex_ring_tb[1] = loadTexture(":/charater/texture/ring_tb_frame_2648_75.png");
    tex_ring_m = loadTexture(":/charater/texture/ring_m_2235_65.png");
}

void GLObj_DataRing::initializeShaders()
{
    loadGenernalShader();
    loadMix2Shader();
}

void GLObj_DataRing::loadGenernalShader()
{
    // sp_general for portrait && ring_m
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
    attr_sp_mix2_Delta_Tex_S = sprogram_mix2->uniformLocation("Delta1f_Tex_S");
}

void GLObj_DataRing::paint()
{
    glAlphaFunc(GL_GREATER, 0.1f);
    glEnable(GL_ALPHA_TEST);
    // Calculate spin angle
    auto elapsed_time = L2dPal::GetDeltaTime();
    ring_spin_angle += ring_spin_rpm * elapsed_time / 60.0f;
    if(ring_spin_angle > 1)
        ring_spin_angle = 0;
    drawRingTB();
    drawRingM();
    glDisable(GL_ALPHA_TEST);
}

void GLObj_DataRing::drawRingTB()
{
    sprogram_mix2->bind();
    sprogram_mix2->setUniformValue(attr_sp_mix2_Delta_Tex_S, ring_spin_angle);
    vao_ring_tb->bind();
    GLEntry::get()->glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    tex_ring_tb[0]->bind();
    GLEntry::get()->glActiveTexture(GL_TEXTURE1);
    glEnable(GL_TEXTURE_2D);
    tex_ring_tb[1]->bind();
    sprogram_mix2->setUniformValue(attr_sp_mix2_Texture_0, 0);
    sprogram_mix2->setUniformValue(attr_sp_mix2_Texture_1, 1);
    QMatrix4x4 tf_ring_tb;
    // Draw ring_t
    sprogram_mix2->setUniformValue(attr_sp_mix2_Delta_Tex_S, ring_spin_angle + 0.5f);
    tf_ring_tb.setToIdentity();
    tf_ring_tb.translate(base_position.x() + 0.0f, base_position.y() + 20.0f, base_position.z());
    tf_ring_tb.rotate(10, 1, 0, 0);
    sprogram_mix2->setUniformValue(attr_sp_mix2_Transform, tf_camera * tf_ring_tb);
    glDrawArrays(GL_QUAD_STRIP, 0, 2 * (ring_tb_res + 1));
    // Draw ring_b
    tf_ring_tb.setToIdentity();
    tf_ring_tb.translate(base_position.x() + 0.0f, base_position.y() - 8.0f, base_position.z());
    tf_ring_tb.rotate(5, 1, 0, 0);
    sprogram_mix2->setUniformValue(attr_sp_mix2_Transform, tf_camera * tf_ring_tb);
    glDrawArrays(GL_QUAD_STRIP, 0, 2 * (ring_tb_res + 1));
    // Clean
    glDisable(GL_TEXTURE_2D);
    tex_ring_tb[0]->release();
    tex_ring_tb[1]->release();
    vao_ring_tb->release();
    sprogram_mix2->release();
}

void GLObj_DataRing::drawRingM()
{
    // Draw ring_m
    sprogram_general->bind();
    vao_ring_m->bind();
    GLEntry::get()->glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    tex_ring_m->bind();
    sprogram_general->setUniformValue(attr_sp_general_Texture_0, 0);
    sprogram_general->setUniformValue(attr_sp_general_Delta_Tex_S, ring_spin_angle);
    QMatrix4x4 tf_ring_m;
    tf_ring_m.setToIdentity();
    tf_ring_m.translate(base_position.x() + 0.0f,  base_position.y() + 5.0f, base_position.z());
    tf_ring_m.rotate(5, 1, 0, 0);
    sprogram_general->setUniformValue(attr_sp_general_Transform, tf_camera * tf_ring_m);
    glDrawArrays(GL_QUAD_STRIP, 0, 2 * (ring_m_res + 1));
    // Clean
    glDisable(GL_TEXTURE_2D);
    tex_ring_m->release();
    vao_ring_m->release();
    sprogram_general->release();
}
