#ifndef IONADESKTOP_CORE_HEXAGON_BASE_H
#define IONADESKTOP_CORE_HEXAGON_BASE_H

#include <QtMath>
#include <QWidget>
#include <QPainterPath>
#define PI 3.1415926535897932384626

namespace IonaDesktop {
namespace Core {
    class HexagonWidgetBase : public QWidget
    {
        Q_OBJECT
    public:
        explicit HexagonWidgetBase(const QRect geo, const double gap = 0, QWidget *parent = nullptr)
            : QWidget (parent), edge_gap(gap)
        { setGeometry(geo); }

        ~HexagonWidgetBase(){}

        void setGap(double gap)
        {
            edge_gap = gap;
            genHexPath();
        }
    private:

    protected:
        double edge_gap;
        QPainterPath hex_path;

        void genHexPath()
        {
            double radius = (width() < height() ? width() / 2 : height() / 2) - edge_gap;
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
            update();
        }

        void resizeEvent(QResizeEvent *ev)
        {
            Q_UNUSED(ev);
              genHexPath();
        }
    };
}
}

#endif // IONADESKTOP_CORE_HEXAGON_BASE_H
