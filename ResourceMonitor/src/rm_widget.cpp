#include "resource_monitor/rm_widget.h"

using namespace IonaDesktop::Core;
using namespace IonaDesktop::ResourceMonitor;

RmWidget::RmWidget(const QRect& geo, const QString &widget_name, QWidget *parent)
    : HexagonWidgetBase(geo, 3, parent),
    rate("0"), name(widget_name)
{
    rate_label_ptr = new QLabel(this);
    rate_label_ptr->setAlignment(Qt::AlignCenter);
    rate_label_ptr->setStyleSheet("font-family:Microsoft YaHei; font-size:27px; color: white;");
    rate_label_ptr->show();

    name_label_ptr = new QLabel(this);
    name_label_ptr->setAlignment(Qt::AlignCenter);
    name_label_ptr->setStyleSheet("font-family:Microsoft YaHei; font-size:12px; color: white;");
    name_label_ptr->show();
}

RmWidget::~RmWidget(){}

void RmWidget::setName(const QString &widget_name)
{ name = widget_name; }
void RmWidget::setRate(const QString &widget_rate)
{ rate = widget_rate; }

void RmWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(70, 90, 139, 135));
    painter.drawPath(hex_path);

    name_label_ptr->setText(name);
    name_label_ptr->setGeometry(0, qRound(0.25 * height()), width(), height());

    rate_label_ptr->setText(rate);
    rate_label_ptr->setGeometry(0, 0, width(), height());
    event->accept();
}
