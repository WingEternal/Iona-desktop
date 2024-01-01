#ifndef IONA_DESKTOP_CORE_DISPLAY_GL_OBJECT_BASE_H
#define IONA_DESKTOP_CORE_DISPLAY_GL_OBJECT_BASE_H

#include <QtGlobal>
#ifdef Q_OS_WIN
#include "Windows.h"
#endif
#include <GL/glu.h>

#include <QObject>
#include <QOpenGLBuffer>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>

#include "gl_handle.h"

namespace IonaDesktop {
namespace CoreDisplay {
class GLObjectBase : public QObject {
  Q_OBJECT
  friend class GLWidget;

 public:
  GLObjectBase(QOpenGLWidget* parent) : QObject(parent) {}
  virtual ~GLObjectBase() {}
  virtual void init() = 0;
  virtual void resize() {}
  virtual void paint() = 0;

 private:
 protected:
};
}  // namespace CoreDisplay
}  // namespace IonaDesktop

#endif  // IONA_DESKTOP_CORE_DISPLAY_GL_OBJECT_BASE_H
