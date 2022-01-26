#ifndef IONADESKTOP_IONA_HITBOX_WIDGET_H
#define IONADESKTOP_IONA_HITBOX_WIDGET_H

#include "core/core_global.h"
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include "core/hexagon_base.h"

namespace IonaDesktop {
namespace Core {
    class MoveWidget : public HexagonWidgetBase
    {
        Q_OBJECT
    public:
        explicit MoveWidget(const QRect& geo, QWidget *parent = nullptr);
        ~MoveWidget();
    private:
        QWidget *move_target;
        QPixmap move_icon;
        bool m_Lpressed;
        QPoint m_pos;
    protected:
        void paintEvent(QPaintEvent *ev);
        void mousePressEvent(QMouseEvent *ev);
        void mouseMoveEvent(QMouseEvent *ev);
        void mouseReleaseEvent(QMouseEvent *ev);
    };
}
}

#endif // IONADESKTOP_IONA_HITBOX_WIDGET_H
