#include "core/base_widget.h"

using namespace IonaDesktop::Core;

void BaseWidget::setupMoveWidget()
{
    move_widget_ptr = new MoveWidget(QRect(450, 300, 50, 50), this);
    move_widget_ptr->show();
}

void BaseWidget::UpdatePosLT()
{
    window_global_posLT.setX(this->geometry().left());
    window_global_posLT.setY(this->geometry().top());
}
