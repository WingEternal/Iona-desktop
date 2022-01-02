#ifndef IONADESKTOP_RM_HEXAGON_WIDGET_PROGRESS_H
#define IONADESKTOP_RM_HEXAGON_WIDGET_PROGRESS_H

#include "resource_monitor/hexagon_widget_base.h"

namespace IonaDesktop {
namespace ResourceMonitor {
    class HexagonProgressWidget : public HexagonWidgetBase
    {
        Q_OBJECT
    public:
        explicit HexagonProgressWidget(const QString &widget_name = "NULL", QWidget *parent = nullptr);
        ~HexagonProgressWidget();
        void setName(const QString &name);
    private:

    protected:
        double rate;
        QLabel rate_label;
        QString name;
        QLabel name_label;

        // genHexagon() can be executed only once because hexagon size dont't change in this plugin
        QPainterPath hexagon_path;
        void paintEvent(QPaintEvent *event);
    public slots:
        void Slot_UpdateRate(const double new_rate);
    };
}
}

#endif // HEXAGON_WIDGET_PROGRESS_H
