#include <QtMath>
#include <QFont>
#include <QPalette>
#include <QPainter>
#include <QPaintEvent>

#include "resource_monitor/hexagon_widget.h"
#define PI 3.1415926535897932384626

using namespace IonaDesktop::ResourceMonitor;

HexagonWidget::HexagonWidget(const QString &widget_name, QWidget *parent) :
    QWidget (parent), rate(0), name(widget_name)
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
HexagonWidget::~HexagonWidget()
{
    delete rate_label_ptr;
    delete name_label_ptr;
}
void HexagonWidget::setName(const QString &widget_name)
{ name = widget_name; }
void HexagonWidget::paintEvent(QPaintEvent *event)
{
    double radius = width() < height() ? width() / 2 : height() / 2;
    // Setup hexagon path
    QPolygonF hexagon = QPolygonF();
    QPointF origin = QPointF(width() / 2, height() / 2);
    hexagon << origin + QPointF(0.5 * radius, sqrt(3) / 2 * radius)
               << origin + QPointF(radius, 0)
               << origin + QPointF(0.5 * radius, - sqrt(3) / 2 * radius)
               << origin + QPointF(- 0.5 * radius, - sqrt(3) / 2 * radius)
               << origin + QPointF(- radius, 0)
               << origin + QPointF(- 0.5 * radius, sqrt(3) / 2 * radius);
    QPainterPath hexagon_path;
    hexagon_path.addPolygon(hexagon);
    hexagon_path.closeSubpath();
    // Setup progress path
    QPolygonF progress = QPolygonF();
    if(rate == 100)
        progress = hexagon;
    else if(rate > 0)
    {
        double degree_angle = 360 * rate / 100;
        progress << origin << origin + QPointF(0, - sqrt(3) / 2 * radius);
        if(degree_angle > 30) {
            progress << origin + QPointF(0.5 * radius, - sqrt(3) / 2 * radius);
            if(degree_angle > 90) {
                progress << origin + QPointF(radius, 0);
                if(degree_angle > 150) {
                    progress << origin + QPointF(0.5 * radius, sqrt(3) / 2 * radius);
                    if(degree_angle > 210) {
                        progress << origin + QPointF(- 0.5 * radius, sqrt(3) / 2 * radius);
                        if(degree_angle > 270) {
                            progress << origin + QPointF(- radius, 0);
                            if(degree_angle > 330)
                                progress << origin + QPointF(- 0.5 * radius, - sqrt(3) / 2 * radius)
                                         << origin + QPointF(- sqrt(3) / 2 * radius * tan((360 - degree_angle) / 180 * PI), - sqrt(3) / 2 * radius);
                            // 270 < degree_angle < 330
                            else {
                                double tmp_var = sqrt(3) / 2 * radius / sin((390 - degree_angle) / 180 * PI);
                                QPointF end_point = origin + QPointF(- tmp_var * cos((degree_angle - 270) / 180 * PI), - tmp_var * sin((degree_angle - 270) / 180 * PI));
                                progress << end_point;
                            }
                        }
                        //  210 < degree_angle < 270
                        else {
                            double tmp_var = sqrt(3) / 2 * radius / sin((-150 + degree_angle) / 180 * PI);
                            QPointF end_point = origin + QPointF(- tmp_var * cos((270 - degree_angle) / 180 * PI), tmp_var * sin((270 - degree_angle) / 180 * PI));
                            progress << end_point;
                        }
                    }
                    // 150 < degree_angle < 210
                    else progress << origin + QPointF(sqrt(3) / 2 * radius * tan((180 - degree_angle) / 180 * PI), sqrt(3) / 2 * radius);
                }
                // 90 <  degree_angle < 150
                else {
                    double tmp_var = sqrt(3) / 2 * radius / sin((210 - degree_angle) / 180 * PI);
                    QPointF end_point = origin + QPointF(tmp_var * cos((degree_angle - 90) / 180 * PI), tmp_var * sin((degree_angle - 90) / 180 * PI));
                    progress << end_point;
                }
            }
            // 30 < degree_angle < 90
            else {
                double tmp_var = sqrt(3) / 2 * radius / sin((30 + degree_angle) / 180 * PI);
                QPointF end_point = origin + QPointF(tmp_var * cos((90 - degree_angle) / 180 * PI), - tmp_var * sin((90 - degree_angle) / 180 * PI));
                progress << end_point;
            }
        }
        // degree_angle < 30
        else progress << origin + QPointF(sqrt(3) / 2 * radius * tan(degree_angle / 180 * PI), - sqrt(3) / 2 * radius);
    }
    QPainterPath progress_path;
    progress_path.addPolygon(progress);
    progress_path.closeSubpath();

    QPen pen;
    QBrush brush;
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    pen.setColor(QColor(39, 50, 85));
    pen.setWidth(3);
    pen.setStyle(Qt::SolidLine);
    pen.setJoinStyle(Qt::RoundJoin);

    brush.setColor(QColor(70, 90, 139, 100));
    brush.setStyle(Qt::SolidPattern);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.drawPath(hexagon_path);

    brush.setColor(QColor(70, 90, 139, 170));
    painter.setBrush(brush);
    painter.drawPath(progress_path);

    name_label_ptr->setText(name);
    name_label_ptr->setGeometry(0, static_cast<int>(0.3 * height()), width(), height());

    rate_label_ptr->setText(QString::number(rate));
    rate_label_ptr->setGeometry(0, 0, width(), height());
    event->accept();
}

void HexagonWidget::Slot_UpdateRate(const int new_rate)
{
    rate = new_rate;
    update();
}
