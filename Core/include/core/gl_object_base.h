#ifndef IONA_DESKTOP_CORE_GL_OBJECT_BASE_H
#define IONA_DESKTOP_CORE_GL_OBJECT_BASE_H

#include <QObject>
#include "core/gl_entry.h"
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <GL/glu.h>

namespace IonaDesktop {
namespace Core {
    class GLObjectBase : public QObject
    {
        Q_OBJECT
    public:
        GLObjectBase(QOpenGLWidget* parent)
            : QObject(parent) {}
        virtual ~GLObjectBase(){}
        virtual void init() = 0;
        virtual void resize(){}
        virtual void paint() = 0;
    private:

    protected:

    signals:
        void RequestUpdate();
    };
}
}

#endif // IONA_DESKTOP_CORE_GL_OBJECT_BASE_H
