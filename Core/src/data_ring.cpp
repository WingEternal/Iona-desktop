#include "core_ex1/data_ring.h"

using namespace IonaDesktop::CoreEx1;
using IonaDesktop::Core::GLEntry;

GLObj_DataRing::GLObj_DataRing(QOpenGLWidget* parent, const QMatrix4x4& tf_camera_)
    : GLObjectBase(parent),
      attr_sp_general_Transform(-1),
      attr_sp_general_Texture_0(-1),
      attr_sp_mix2_Transform(-1),
      attr_sp_mix2_Texture_0(-1),
      attr_sp_mix2_Texture_1(-1),
      attr_sp_mix2_Delta_Tex_S(-1),
      tex_ring_tb{nullptr, nullptr},
      tex_ring_m(nullptr),
      base_position(0, 0, -75.0),
      tf_camera(tf_camera_),
      ring_spin_delta_angle(0.5),
      ring_spin_angle(0)
{

}
GLObj_DataRing::~GLObj_DataRing()
{
    for(int i = 0; i < 2; i++)
        tex_ring_tb[i]->destroy();
    tex_ring_m->destroy();
}

void GLObj_DataRing::init()
{
    ring_spin_update_timer_ptr = new QTimer(this);
    connect(ring_spin_update_timer_ptr, SIGNAL(timeout()), this, SLOT(Slot_RingSpinUpdateTimer_Timeout()));
    ring_spin_update_timer_ptr->start(ring_update_duration);

    initializeTexture();
    initializeShaders();
    initializeModel();
}

void GLObj_DataRing::paint()
{
    glAlphaFunc(GL_GREATER, 0.1f);
    glEnable(GL_ALPHA_TEST);
    drawRingTB();
    drawRingM();
    glDisable(GL_ALPHA_TEST);
}

void GLObj_DataRing::drawRingTB()
{
    GLfloat delta_tex = ring_spin_angle / 360.0;
    // Draw ring_tb
     sprogram_mix2->bind();
     sprogram_mix2->setUniformValue(attr_sp_mix2_Delta_Tex_S, delta_tex);
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
     sprogram_mix2->setUniformValue(attr_sp_mix2_Delta_Tex_S, GLfloat(delta_tex + 0.5));
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
    QMatrix4x4 tf_ring_m;
    tf_ring_m.setToIdentity();
    tf_ring_m.translate(base_position.x() + 0.0f,  base_position.y() + 5.0f, base_position.z());
    tf_ring_m.rotate(5, 1, 0, 0);
    tf_ring_m.rotate(- ring_spin_angle, 0, 1, 0);
    sprogram_general->setUniformValue(attr_sp_general_Transform, tf_camera * tf_ring_m);
    glDrawArrays(GL_QUAD_STRIP, 0, 2 * (ring_m_res + 1));
    // Clean
    glDisable(GL_TEXTURE_2D);
    tex_ring_m->release();
    vao_ring_m->release();
    sprogram_general->release();
}

void GLObj_DataRing::mousePressEvent(QMouseEvent *ev)
{
    ring_spin_delta_angle = 1;
}

void GLObj_DataRing::mouseMoveEvent(QMouseEvent *ev)
{

}

void GLObj_DataRing::mouseReleaseEvent(QMouseEvent *ev)
{
    ring_spin_delta_angle = 0.5;
}

void GLObj_DataRing::Slot_RingSpinUpdateTimer_Timeout()
{
    ring_spin_angle += ring_spin_delta_angle;
    if(abs(ring_spin_angle - 360) < 1e-6)
        ring_spin_angle = 0;
    emit RequestUpdate();
}
