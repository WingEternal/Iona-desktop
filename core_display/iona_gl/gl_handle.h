#ifndef IONA_DESKTOP_CORE_DISPLAY_GL_FUNCTIONS_H
#define IONA_DESKTOP_CORE_DISPLAY_GL_FUNCTIONS_H

#include <QOpenGLFunctions>

namespace IonaDesktop {
namespace CoreDisplay {
class GLHandle : public QOpenGLFunctions {
 public:
  inline static QOpenGLFunctions* get() {
    if (!instance) instance = new QOpenGLFunctions;
    return instance;
  }
  inline static void release() {
    if (!instance) delete instance;
    instance = nullptr;
  }

 private:
  GLHandle() {}
  ~GLHandle() {}
  static QOpenGLFunctions* instance;
};
}  // namespace CoreDisplay
}  // namespace IonaDesktop

#endif  //  IONA_DESKTOP_CORE_DISPLAY_GL_FUNCTIONS_H
