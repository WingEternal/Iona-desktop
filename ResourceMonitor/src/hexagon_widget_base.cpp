#include <QtMath>

#include "resource_monitor/hexagon_widget_base.h"
#define PI 3.1415926535897932384626

using namespace IonaDesktop::ResourceMonitor;

HexagonWidgetBase::HexagonWidgetBase(QWidget *parent) : QWidget (parent){}
HexagonWidgetBase::~HexagonWidgetBase(){}

void HexagonWidgetBase::updateParams()
{
    radius = width() < height() ? width() / 2 : height() / 2;
    origin = QPointF(width() / 2, height() / 2);
    hexagon << origin + QPointF(0.5 * radius, sqrt(3) / 2 * radius)
               << origin + QPointF(radius, 0)
               << origin + QPointF(0.5 * radius, - sqrt(3) / 2 * radius)
               << origin + QPointF(- 0.5 * radius, - sqrt(3) / 2 * radius)
               << origin + QPointF(- radius, 0)
               << origin + QPointF(- 0.5 * radius, sqrt(3) / 2 * radius);
}
const QPolygonF* HexagonWidgetBase::getHexagonPoints() const
{ return &hexagon; }
void HexagonWidgetBase::genHexagonPath(QPainterPath &path)
{
    updateParams();
    path.addPolygon(hexagon);
    path.closeSubpath();
}

void HexagonWidgetBase::genHexagonProgressPath(QPainterPath &path, const double progress_rate)
{
    updateParams();
    QPolygonF progress = QPolygonF();
    if(abs(progress_rate - 100) < 1e-5)
        progress = hexagon;
    else if(progress_rate > 0)
    {
        double degree_angle = 360 * progress_rate / 100;
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
    path.addPolygon(progress);
    path.closeSubpath();
}


