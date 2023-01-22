#ifndef IONA_DESKTOP_CORE_DISPLAY_GL_OBJECT_BASE_H
#define IONA_DESKTOP_CORE_DISPLAY_GL_OBJECT_BASE_H

#include <QObject>
#include "gl_entry.h"
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <GL/glu.h>

namespace IonaDesktop {
namespace CoreDisplay {
    class GLObjectBase : public QObject
    {
        Q_OBJECT
        friend class GLWidget;
    public:
        GLObjectBase(QOpenGLWidget* parent)
            : QObject(parent) {}
        virtual ~GLObjectBase(){}
        virtual void init() = 0;
        virtual void resize(){}
        virtual void paint() = 0;

    private:

    protected:
        virtual void mousePressEvent(QMouseEvent *ev){ Q_UNUSED(ev); }
        virtual void mouseMoveEvent(QMouseEvent *ev){ Q_UNUSED(ev); }
        virtual void mouseReleaseEvent(QMouseEvent *ev){ Q_UNUSED(ev); }
    };
}
}

#endif // IONA_DESKTOP_CORE_DISPLAY_GL_OBJECT_BASE_H
