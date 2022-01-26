#include "core/gl_widget.h"

using namespace IonaDesktop::Core;

void GLWidget::loadTexture(QOpenGLTexture **tex_ptr2, const QString &address)
{
    if((*tex_ptr2) != nullptr)
    {
        delete *tex_ptr2;
        *tex_ptr2 = nullptr;
    }
    *tex_ptr2 = new QOpenGLTexture(QImage(address).mirrored(), QOpenGLTexture::GenerateMipMaps);
    (*tex_ptr2)->setMinificationFilter(QOpenGLTexture::Linear);
    (*tex_ptr2)->setMagnificationFilter(QOpenGLTexture::Linear);
    (*tex_ptr2)->setWrapMode(QOpenGLTexture::Repeat);
}

void GLWidget::initializeTexture()
{
    loadTexture(&tex_portrait, ":/charater/texture/Iona_514_723.png");
    loadTexture(&tex_ring_tb[0], ":/charater/texture/ring_tb_block_2648_75.png");
    loadTexture(&tex_ring_tb[1], ":/charater/texture/ring_tb_frame_2648_75.png");
    loadTexture(&tex_ring_m, ":/charater/texture/ring_m_2235_65.png");
}
