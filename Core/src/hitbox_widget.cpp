#include "core/hitbox_widget.h"

using namespace IonaDesktop::Core;

HitboxWidget::HitboxWidget(QWidget *parent)
    : QWidget (parent)
{

}

HitboxWidget::~HitboxWidget()
{

}

void HitboxWidget::paintEvent(QPaintEvent *ev)
{
#ifdef DEBUG_HITBOX_RANGE
    QPainter painter(this);
    painter.setPen(Qt::red);
    painter.drawRect(0, 0, width() - 1, height() - 1);
#endif
    ev->accept();
}

void HitboxWidget::mousePressEvent(QMouseEvent *ev)
{
    emit mouseClicked();
    ev->accept();
}
