#include "core/base_widget.h"

using namespace IonaDesktop::Core;

GLWidget* BaseWidget::getDisplayInstance()
{
    return gl_widget_ptr;
}

void BaseWidget::setupGLWidget()
{
    gl_widget_ptr = new GLWidget(QRect(0, 0, this->width(), this->height()), this);
    gl_widget_ptr->lower();
    gl_widget_ptr->show();
}
