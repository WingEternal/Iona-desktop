#include "core/gl_widget.h"

using namespace IonaDesktop::Core;

GLWidget::GLWidget(const QRect& geo, QWidget *parent)
    : QOpenGLWidget(parent),
      widget_geo(geo)
{
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setAttribute(Qt::WA_TransparentForMouseEvents);
}
GLWidget::~GLWidget()
{
    makeCurrent();

    sprogram_general->destroyed();
    sprogram_mix2->destroyed();

    tex_portrait->destroy();
    for(int i = 0; i < 2; i++)
        tex_ring_tb[i]->destroy();

    // Destory VAO
    vao_portrait->destroy();
    vao_ring_tb->destroy();
    vao_ring_m->destroy();
}
