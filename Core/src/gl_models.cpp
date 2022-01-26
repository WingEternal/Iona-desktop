#include "core/gl_widget.h"
#include <QtMath>
#define PI 3.1415926536

using namespace IonaDesktop::Core;

namespace IonaDesktop {
namespace Core {
    /* DO NOT EDIT */
    constexpr GLfloat portrait_width = portrait_height * 514 / 723;
    constexpr GLfloat vertices_portrait[] =
    {
        // -------position------     ---tex---
        - portrait_width / 2, + portrait_height / 2, 0.0,    0.0, 1.0,
        - portrait_width / 2, - portrait_height / 2, 0.0,    0.0, 0.0,
       + portrait_width / 2, - portrait_height / 2, 0.0,   1.0, 0.0,
       + portrait_width / 2, + portrait_height / 2, 0.0,   1.0, 1.0
    };
    //const GLuint indices_portrait[] = {0, 1, 2, 3};

    // Ring circumference
    constexpr GLfloat ring_tb_perimeter = ring_tb_height * 17650 / 540;
    constexpr GLfloat ring_m_perimeter = ring_m_height * 18564 / 540;
    // Ring Radius (according to ring texture resolution)
    constexpr GLfloat ring_tb_radius = ring_tb_perimeter / 2 /  PI;
    constexpr GLfloat ring_m_radius = ring_m_perimeter / 2 /  PI;
    constexpr GLint ring_tb_vertices_size = 2 * 5 * (ring_tb_res + 1);
    constexpr GLint ring_m_vertices_size = 2 * 5 * (ring_m_res + 1);
    static GLfloat vertices_ring_tb[ring_tb_vertices_size];
    static GLfloat vertices_ring_m[ring_m_vertices_size];
    //GLuint indices_ring_tb[2 * ring_tb_res + 2];

    void genRingVertices(GLfloat *container_array, const GLint size, const GLfloat radius, const GLfloat height, const int res)
    {
        if(!container_array || size < 2 * 5 * (res + 1)) { qDebug() << "Not enough memory allocated"; return; }
        const GLfloat angle_incremental = 360.0f / res;
        // Use N = (Resolution +1) to form a closed loop
        for(int i = 0; i < res; i ++)
        {
            GLfloat angle = angle_incremental * i * 3.14 / 180;
            // vertices on top
            // Position
            container_array[i * 10] = radius * sin(angle);
            container_array[i * 10 + 1] = height / 2;
            container_array[i * 10 + 2] = radius * cos(angle);
            // Texture coordinates
            container_array[i * 10 + 3] = angle_incremental * i / 360;
            container_array[i * 10 + 4] = 1;

            // vertices below
            container_array[i * 10 + 5] = radius * sin(angle);
            container_array[i * 10 + 6] = - height / 2;
            container_array[i * 10 + 7] = radius * cos(angle);
            container_array[i * 10 + 8] = angle_incremental * i / 360;
            container_array[i * 10 + 9] = 0;
        }
        // Manually add the last point (void precision errors)
        container_array[size - 10] = 0;
        container_array[size - 9] = height / 2;
        container_array[size - 8] = radius;
        container_array[size - 7] = 1;
        container_array[size - 6] = 1;
        container_array[size - 5] = 0;
        container_array[size - 4] = - height / 2;
        container_array[size - 3] = radius;
        container_array[size - 2] = 1;
        container_array[size - 1] = 0;
    }
}
}

void GLWidget::initializeModel()
{
    // Portrait
    vao_portrait = new QOpenGLVertexArrayObject(this);
    loadModel(vao_portrait, sprogram_general, vertices_portrait, sizeof (vertices_portrait));

    // Ring_tb
    genRingVertices(vertices_ring_tb, ring_tb_vertices_size, ring_tb_radius, ring_tb_height, ring_tb_res);
    vao_ring_tb = new QOpenGLVertexArrayObject(this);
    loadModel(vao_ring_tb, sprogram_mix2, vertices_ring_tb, sizeof (vertices_ring_tb));

    // Ring_m
    genRingVertices(vertices_ring_m, ring_m_vertices_size, ring_m_radius, ring_m_height, ring_m_res);
    vao_ring_m = new QOpenGLVertexArrayObject(this);
    loadModel(vao_ring_m, sprogram_general, vertices_ring_m, sizeof (vertices_ring_m));
}

void GLWidget::loadModel(QOpenGLVertexArrayObject *vao, QOpenGLShaderProgram *sp, const GLfloat *vertices, const size_t vertices_length)
{
    QOpenGLVertexArrayObject::Binder bind_vao(vao);
    QOpenGLBuffer vbo(QOpenGLBuffer::VertexBuffer);
    vbo.create();
    vbo.bind();
    vbo.allocate(vertices, vertices_length);
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    auto attr = -1;
    attr = sp->attributeLocation("Pos4f");
    sp->setAttributeBuffer(attr, GL_FLOAT, 0, 3, 5 * sizeof (GLfloat));
    sp->enableAttributeArray(attr);
    attr = sp->attributeLocation("TexCoord2f_0");
    sp->setAttributeBuffer(attr, GL_FLOAT, 3 * sizeof (GLfloat), 2, 5 * sizeof (GLfloat));
    sp->enableAttributeArray(attr);
    // Clean
    vbo.release();
    vao->release();
    vbo.destroy();
}
