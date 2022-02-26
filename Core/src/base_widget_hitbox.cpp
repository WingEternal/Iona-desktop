#include "core/base_widget.h"

using namespace IonaDesktop::Core;

void BaseWidget::setupHitboxWidgets()
{
    head_hitbox_widget = new HitboxWidget(this);
    head_hitbox_widget->setGeometry(260, 80, 70, 60);
    connect(head_hitbox_widget, SIGNAL(mouseClicked()), this, SLOT(Slot_Hitbox_Hit()));
    head_hitbox_widget->show();

    body_hitbox_widget = new HitboxWidget(this);
    body_hitbox_widget->setGeometry(290, 135, 80, 110);
    connect(body_hitbox_widget, SIGNAL(mouseClicked()), this, SLOT(Slot_Hitbox_Hit()));
    body_hitbox_widget->show();
}
