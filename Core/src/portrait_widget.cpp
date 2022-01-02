#include <QBitmap>
#include <QPixmap>
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>

#include "core/core_global.h"
#include "core/portrait_widget.h"

using namespace IonaDesktop::Core;

PortraitWidget::PortraitWidget(QWidget *parent)
    : QWidget (parent){}
PortraitWidget::~PortraitWidget(){}
void PortraitWidget::paintEvent(QPaintEvent *ev)
{
    QPainter painter(this);
    QPixmap pixmap, pixmapScaled;
    pixmap.load(":/charater/image/Iona.png");
    pixmapScaled = pixmap.scaled(width(), height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    painter.drawPixmap(0, 0, pixmapScaled);
    qDebug() << "Iona - drawSized: " << pixmapScaled.rect();
    setFixedSize(pixmapScaled.size());
    setMask(pixmapScaled.mask());
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
