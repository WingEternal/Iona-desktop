#ifndef IONADESKTOP_RM_HEXAGON_WIDGET_BASE_H
#define IONADESKTOP_RM_HEXAGON_WIDGET_BASE_H

#include <QWidget>
#include <QLabel>

namespace IonaDesktop {
namespace ResourceMonitor {
    class HexagonWidgetBase : public QWidget
    {
        Q_OBJECT
    public:
        explicit HexagonWidgetBase(QWidget *parent = nullptr);
        ~HexagonWidgetBase();
    private:
        double radius;
        QPointF origin;
        QPolygonF hexagon;
        void updateParams();
        const QPolygonF* getHexagonPoints() const;
    protected:
        void genHexagonPath(QPainterPath& path);
        void genHexagonProgressPath(QPainterPath& path, const double progress_rate);
    };
}
}


#endif // IONADESKTOP_RM_HEXAGON_WIDGET_BASE_H
