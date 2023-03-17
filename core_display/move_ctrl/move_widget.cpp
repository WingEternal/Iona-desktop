#include "move_ctrl/move_widget.h"
#include "app/app_msg_handler.h"
#include <QPaintEvent>
#include <QPainter>
#include <QMouseEvent>

using namespace IonaDesktop::CoreDisplay;

MoveWidget::MoveWidget(QWidget *parent)
    : QWidget(parent),
      move_target(parent)
{
    this->setGeometry(0.75 * parent->width(), 0.7 * parent->height(), 50, 50);
    icon.load(":/icon/move.png");

    AppMsgHandler::getInstance().regSignal
        ("/app/set_param", this, SIGNAL(setParam(const QString, const QVariant)));
}

MoveWidget::~MoveWidget()
{

}

void MoveWidget::paintEvent(QPaintEvent* ev)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, icon);
    ev->accept();
}

void MoveWidget::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton) {
        m_pos = ev->globalPosition().toPoint();
        m_Lpressed = true;
    }
    ev->accept();
}

void MoveWidget::mouseMoveEvent(QMouseEvent *ev)
{
    if(m_Lpressed) {
        QPoint step = ev->globalPosition().toPoint() - m_pos;
        //move_target->setGeometry(QRect(step + move_target->geometry().topLeft(), move_target->size()));
        move_target->move(step + move_target->geometry().topLeft());
        m_pos += step;
    }
    ev->accept();
}

void MoveWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    m_Lpressed = false;
    emit setParam("S$window/pos", move_target->pos());
    emit setParam("D$window/center", move_target->geometry().center());
    ev->accept();
}

