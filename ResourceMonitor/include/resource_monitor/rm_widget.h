#ifndef IONADESKTOP_RM_HEXAGON_WIDGET_PROGRESS_H
#define IONADESKTOP_RM_HEXAGON_WIDGET_PROGRESS_H

#include "core/hexagon_base.h"
#include <QLabel>
#include <QPainter>
#include <QPaintEvent>

namespace IonaDesktop {
namespace ResourceMonitor {
    class RmWidget : public IonaDesktop::Core::HexagonWidgetBase
    {
        Q_OBJECT
    public:
        explicit RmWidget(const QRect& geo, const QString &widget_name = "NULL", QWidget *parent = nullptr);
        ~RmWidget();

        void setName(const QString &widget_name);
        void setRate(const QString &widget_rate);
    private:

    protected:
        QString rate;
        QLabel *rate_label_ptr;
        QString name;
        QLabel *name_label_ptr;
        void paintEvent(QPaintEvent *event);
    };
}
}

#endif // HEXAGON_WIDGET_PROGRESS_H
