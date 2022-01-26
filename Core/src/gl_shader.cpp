#include "core/gl_widget.h"

using namespace IonaDesktop::Core;

void GLWidget::initializeShaders()
{
    loadGenernalShader();
    loadMix2Shader();
}

void GLWidget::loadGenernalShader()
{
    // sp_general for portrait && ring_m
    sprogram_general = new QOpenGLShaderProgram(this);
    sprogram_general->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/charater/shader/vs_general.vert");
    sprogram_general->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/charater/shader/fs_general.frag");
    if(!sprogram_general->link()) {
        qDebug() << "General Shader Program Link Failed";
        close();
    }
    attr_sp_general_Transform = sprogram_general->uniformLocation("Transform");
    attr_sp_general_Texture_0 = sprogram_general->uniformLocation("Texture_0");
}

void GLWidget::loadMix2Shader()
{
    // sp_mix2 for ring_tb
    sprogram_mix2 = new QOpenGLShaderProgram(this);
    sprogram_mix2->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/charater/shader/vs_general.vert");
    sprogram_mix2->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/charater/shader/fs_mix2.frag");
    if(!sprogram_mix2->link()) {
        qDebug() << "Mix2 Shader Program Link Failed";
        close();
    }
    attr_sp_mix2_Transform = sprogram_mix2->uniformLocation("Transform");
    attr_sp_mix2_Texture_0 = sprogram_mix2->uniformLocation("Texture_0");
    attr_sp_mix2_Texture_1 = sprogram_mix2->uniformLocation("Texture_1");
    attr_sp_mix2_Delta_Tex_S = sprogram_mix2->uniformLocation("Delta1f_Tex_S");
}
