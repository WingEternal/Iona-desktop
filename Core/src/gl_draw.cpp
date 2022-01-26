#include "core/gl_widget.h"

using namespace IonaDesktop::Core;

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    setGeometry(widget_geo);
    attr_sp_general_Transform = -1;
    attr_sp_general_Texture_0 = -1;
    attr_sp_mix2_Transform = -1;
    attr_sp_mix2_Texture_0 = -1;
    attr_sp_mix2_Texture_1 = -1;
    attr_sp_mix2_Delta_Tex_S = -1;
    tex_portrait = nullptr;
    tex_ring_tb[0] = nullptr;
    tex_ring_tb[1] = nullptr;
    tex_ring_m = nullptr;
    ring_spin_angle = 0;

    ring_spin_update_timer = new QTimer(this);
    connect(ring_spin_update_timer, SIGNAL(timeout()), this, SLOT(Slot_RingSpinUpdateTimer_Timeout()));
    ring_spin_update_timer->start(50);

   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
   glShadeModel(GL_SMOOTH);
   glClearDepth(1.0);
   glEnable(GL_DEPTH_TEST);
   glAlphaFunc(GL_GREATER, 0.1f);
   glEnable(GL_ALPHA_TEST);
   initializeTexture();
   initializeShaders();
   initializeModel();
}
void GLWidget::resizeGL(int w, int h)
{
    GLfloat fov = 50.0f;
    GLfloat aspect = GLfloat(w) / GLfloat(h);
    GLfloat zMin = 0.1f, zMax = 120.0f;
    tf_camera.setToIdentity();
    tf_camera.perspective(fov, aspect, zMin, zMax);
}

void GLWidget::paintGL()
{

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

   drawPortrait();
   drawRingTB();
   drawRingM();
}

void GLWidget::drawPortrait()
{
    // Draw portrait
    sprogram_general->bind();
    vao_portrait->bind();
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    tex_portrait->bind();
    sprogram_general->setUniformValue(attr_sp_general_Texture_0, 0);
    QMatrix4x4 tf_portrait;
    tf_portrait.setToIdentity();
    tf_portrait.translate(iona_position);
    sprogram_general->setUniformValue(attr_sp_general_Transform, tf_camera * tf_portrait);
    glDrawArrays(GL_QUADS, 0, 4);
 //   glDrawArrays(GL_TRIANGLES, 0, 3);
 //   glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr);
 //   glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, nullptr);
    // Clean
    glDisable(GL_TEXTURE_2D);
    tex_portrait->release();
    vao_portrait->release();
    sprogram_general->release();
}

void GLWidget::drawRingTB()
{
    GLfloat delta_tex = ring_spin_angle / 360.0;
    // Draw ring_tb
     sprogram_mix2->bind();
     sprogram_mix2->setUniformValue(attr_sp_mix2_Delta_Tex_S, delta_tex);
     vao_ring_tb->bind();
     glActiveTexture(GL_TEXTURE0);
     glEnable(GL_TEXTURE_2D);
     tex_ring_tb[0]->bind();
     glActiveTexture(GL_TEXTURE1);
     glEnable(GL_TEXTURE_2D);
     tex_ring_tb[1]->bind();
     sprogram_mix2->setUniformValue(attr_sp_mix2_Texture_0, 0);
     sprogram_mix2->setUniformValue(attr_sp_mix2_Texture_1, 1);
     QMatrix4x4 tf_ring_tb;
     // Draw ring_t
     sprogram_mix2->setUniformValue(attr_sp_mix2_Delta_Tex_S, GLfloat(delta_tex + 0.5));
     tf_ring_tb.setToIdentity();
     tf_ring_tb.translate(iona_position.x() + 0.0f, iona_position.y() + 20.0f, iona_position.z());
     tf_ring_tb.rotate(10, 1, 0, 0);
     sprogram_mix2->setUniformValue(attr_sp_mix2_Transform, tf_camera * tf_ring_tb);
     glDrawArrays(GL_QUAD_STRIP, 0, 2 * (ring_tb_res + 1));
     // Draw ring_b
     tf_ring_tb.setToIdentity();
     tf_ring_tb.translate(iona_position.x() + 0.0f, iona_position.y() - 8.0f, iona_position.z());
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

void GLWidget::drawRingM()
{
    // Draw ring_m
    sprogram_general->bind();
    vao_ring_m->bind();
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    tex_ring_m->bind();
    sprogram_general->setUniformValue(attr_sp_general_Texture_0, 0);
    QMatrix4x4 tf_ring_m;
    tf_ring_m.setToIdentity();
    tf_ring_m.translate(iona_position.x() + 0.0f,  iona_position.y() + 5.0f, iona_position.z());
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

void GLWidget::Slot_RingSpinUpdateTimer_Timeout()
{
    ring_spin_angle += 0.5;
    if(abs(ring_spin_angle - 360) < 1e-6)
        ring_spin_angle = 0;
    update();
}
