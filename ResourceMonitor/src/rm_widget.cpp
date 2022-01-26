#include "resource_monitor/rm_widget.h"

using namespace IonaDesktop::Core;
using namespace IonaDesktop::ResourceMonitor;

RmWidget::RmWidget(const QRect& geo, const QString &widget_name, QWidget *parent)
    : HexagonWidgetBase(geo, 3, parent),
    rate(0), name(widget_name)
{
    rate_label_ptr = new QLabel(this);
    rate_label_ptr->setAlignment(Qt::AlignCenter);
    rate_label_ptr->setStyleSheet("font-family:微软雅黑; font-size:30px; color: white;");
    rate_label_ptr->show();

    name_label_ptr = new QLabel(this);
    name_label_ptr->setAlignment(Qt::AlignCenter);
    name_label_ptr->setStyleSheet("font-family:微软雅黑; font-size:12px; color: white;");
    name_label_ptr->show();
}
RmWidget::~RmWidget(){}
void RmWidget::setName(const QString &widget_name)
{ name = widget_name; }
void RmWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(70, 90, 139, 135));
    painter.drawPath(hex_path);

    name_label_ptr->setText(name);
    name_label_ptr->setGeometry(0, qRound(0.3 * height()), width(), height());

    rate_label_ptr->setText(QString("%1").arg(QString::number(rate, 'g', 3)));
    rate_label_ptr->setGeometry(0, 0, width(), height());
    event->accept();
}

void RmWidget::Slot_UpdateRate(const double new_rate)
{
    rate = new_rate;
    update();
}
