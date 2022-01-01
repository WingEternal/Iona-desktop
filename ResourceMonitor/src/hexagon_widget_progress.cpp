#include <QFont>
#include <QPalette>
#include <QPainter>
#include <QPaintEvent>

#include "resource_monitor/hexagon_widget_progress.h"

using namespace IonaDesktop::ResourceMonitor;

HexagonProgressWidget::HexagonProgressWidget(const QString &widget_name, QWidget *parent) :
    HexagonWidgetBase(parent), rate(0), name(widget_name)
{
    QPalette general_label_palette;
    general_label_palette.setColor(QPalette::WindowText, Qt::white);

    QFont rate_font;
    rate_font.setFamily("微软雅黑");
    rate_font.setPixelSize(30);
    rate_label_ptr = new QLabel(this);
    rate_label_ptr->setAlignment(Qt::AlignCenter);
    rate_label_ptr->setPalette(general_label_palette);
    rate_label_ptr->setFont(rate_font);
    rate_label_ptr->show();

    QFont name_font;
    name_font.setFamily("微软雅黑");
    name_font.setPixelSize(12);
    name_label_ptr = new QLabel(this);
    name_label_ptr->setAlignment(Qt::AlignCenter);
    name_label_ptr->setPalette(general_label_palette);
    name_label_ptr->setFont(name_font);
    name_label_ptr->show();
}
HexagonProgressWidget::~HexagonProgressWidget()
{
    delete rate_label_ptr;
    delete name_label_ptr;
}
void HexagonProgressWidget::setName(const QString &widget_name)
{ name = widget_name; }
void HexagonProgressWidget::paintEvent(QPaintEvent *event)
{
    QPainterPath hex_path, hex_progress_path;
    genHexagonPath(hex_path);
    genHexagonProgressPath(hex_progress_path, rate);

    QPen pen;
    QBrush brush;
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    brush.setColor(QColor(70, 90, 139, 100));
    brush.setStyle(Qt::SolidPattern);
    painter.setPen(Qt::NoPen);
    painter.setBrush(brush);
    painter.drawPath(hex_path);

    pen.setColor(QColor(39, 50, 85));
    pen.setWidth(3);
    pen.setStyle(Qt::SolidLine);
    pen.setJoinStyle(Qt::RoundJoin);
    brush.setColor(QColor(70, 90, 139, 170));
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.drawPath(hex_progress_path);

    name_label_ptr->setText(name);
    name_label_ptr->setGeometry(0, static_cast<int>(0.3 * height()), width(), height());

    rate_label_ptr->setText(QString("%1").arg(QString::number(rate, 'g', 3)));
    rate_label_ptr->setGeometry(0, 0, width(), height());
    event->accept();
}

void HexagonProgressWidget::Slot_UpdateRate(const double new_rate)
{
    rate = new_rate;
    update();
}
