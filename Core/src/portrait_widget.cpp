
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>

#include "core/core_global.h"
#include "core/portrait_widget.h"

using namespace IonaDesktop::Core;

PortraitWidget::PortraitWidget(QWidget *parent)
    : QWidget (parent)
{
    QPixmap pix_origin;
    pix_origin.load(":/charater/image/Iona.png");
    pix_portrait = pix_origin.scaled(500, 500, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    mask_portrait = pix_portrait.mask();
}
PortraitWidget::~PortraitWidget(){}
void PortraitWidget::paintEvent(QPaintEvent *ev)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, pix_portrait);
    qDebug() << "portrait-draw size: " << pix_portrait.rect();
    setMask(mask_portrait);
#ifdef DEBUG_WINDOW_POSITION
    QPainter painter(this);
    painter.setPen(Qt::white);
    painter.drawRect(0, 0, width() - 1, height() - 1);
#endif
    ev->accept();
}
void PortraitWidget::mousePressEvent(QMouseEvent *ev)
{
    emit mousePress();
    ev->accept();
}
