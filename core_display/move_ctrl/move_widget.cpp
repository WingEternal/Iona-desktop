#include "move_widget.h"
#include "app_config.h"
#include "app_msg_handler.h"
#include <QPaintEvent>
#include <QPainter>
#include <QMouseEvent>

using namespace IonaDesktop::CoreDisplay;

MoveWidget::MoveWidget(QWidget *parent)
    : QWidget(parent),
      move_target(parent)
{
    QRect geo;
    AppConfig::getInstance().getParam("/move_ctrl/size", geo);
    this->setGeometry(geo);

    double radius = width() < height() ? width() / 2 : height() / 2;
    if(radius < 1)  radius = 1;
    QPointF local_origin = QPointF(width() / 2, height() / 2);
    QPolygonF hex_poly;
    hex_poly << local_origin + QPointF(0.5 * radius, sqrt(3) / 2 * radius)
        << local_origin + QPointF(radius, 0)
        << local_origin + QPointF(0.5 * radius, - sqrt(3) / 2 * radius)
        << local_origin + QPointF(- 0.5 * radius, - sqrt(3) / 2 * radius)
        << local_origin + QPointF(- radius, 0)
        << local_origin + QPointF(- 0.5 * radius, sqrt(3) / 2 * radius);
    hex_path.addPolygon(hex_poly);
    hex_path.closeSubpath();

    QPixmap move_icon_ori;
    move_icon_ori.load(":/icon/move.png");
    const double ratio = 0.4;
    move_icon = move_icon_ori.scaled(qRound(ratio * geo.width()), qRound(ratio * geo.height()), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    AppMsgHandler::getInstance().regSignal
        ("/app/set_param", this, SIGNAL(saveGeometry(QString, QVariant)));
}

MoveWidget::~MoveWidget()
{

}

void MoveWidget::paintEvent(QPaintEvent* ev)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor(70, 90, 139, 135));
    painter.setPen(Qt::NoPen);
    painter.drawPath(hex_path);
    painter.drawPixmap((this->width() - move_icon.width()) / 2, (this->height() - move_icon.height()) / 2, move_icon);
    ev->accept();
}

void MoveWidget::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton) {
        m_pos = ev->globalPos();
        m_Lpressed = true;
    }
    ev->accept();
}

void MoveWidget::mouseMoveEvent(QMouseEvent *ev)
{
    if(m_Lpressed) {
        QPoint step = ev->globalPos() - m_pos;
        //move_target->setGeometry(QRect(step + move_target->geometry().topLeft(), move_target->size()));
        move_target->move(step + move_target->geometry().topLeft());
        m_pos += step;
    }
    ev->accept();
}

void MoveWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    m_Lpressed = false;
    emit saveGeometry("/window/size", move_target->geometry());
    ev->accept();
}

