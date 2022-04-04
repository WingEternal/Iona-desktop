#include "core_ex1/data_ring.h"

using namespace IonaDesktop::CoreEx1;

QOpenGLTexture * GLObj_DataRing::loadTexture(const QString &address)
{
    QOpenGLTexture* tex_ptr = new QOpenGLTexture(QImage(address).mirrored(), QOpenGLTexture::GenerateMipMaps);
    tex_ptr->setMinificationFilter(QOpenGLTexture::Linear);
    tex_ptr->setMagnificationFilter(QOpenGLTexture::Linear);
    tex_ptr->setWrapMode(QOpenGLTexture::Repeat);
    return  tex_ptr;
}

void GLObj_DataRing::initializeTexture()
{
    tex_ring_tb[0] = loadTexture(":/charater/texture/ring_tb_block_2648_75.png");
    tex_ring_tb[1] = loadTexture(":/charater/texture/ring_tb_frame_2648_75.png");
    tex_ring_m = loadTexture(":/charater/texture/ring_m_2235_65.png");
}
