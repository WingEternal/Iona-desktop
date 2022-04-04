#include "core_ex1/data_ring.h"

using namespace IonaDesktop::CoreEx1;

namespace IonaDesktop {
namespace CoreEx1 {
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
            varying vec2 Tex2f_0; \
            void main(void) \
            { \
                gl_FragColor = texture2D(Texture_0, Tex2f_0.st); \
            }";
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
