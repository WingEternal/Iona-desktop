#include "core/move_widget.h"

using namespace IonaDesktop::Core;

MoveWidget::MoveWidget(const QRect& geo, QWidget *parent)
    : HexagonWidgetBase(geo, 0, parent),
      move_target(parent)
{
    QPixmap move_icon_ori;
    move_icon_ori.load(":/icon/move.png");
    const double ratio = 0.4;
    move_icon = move_icon_ori.scaled(qRound(ratio * geo.width()), qRound(ratio * geo.height()), Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

MoveWidget::~MoveWidget()
{

}

void MoveWidget::paintEvent(QPaintEvent *ev)
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
}

void MoveWidget::mouseMoveEvent(QMouseEvent *ev)
{
    if(m_Lpressed) {
        QPoint step = ev->globalPos() - m_pos;
        move_target->setGeometry(QRect(step + move_target->geometry().topLeft(), move_target->size()));
        m_pos += step;
    }
}

void MoveWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev);
    m_Lpressed = false;
}
