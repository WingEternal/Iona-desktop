#include "core/base_widget.h"

using namespace IonaDesktop::Core;

void BaseWidget::setupMoveWidget()
{
    move_widget_ptr = new MoveWidget(QRect(450, 300, 50, 50), this);
    move_widget_ptr->show();
}
