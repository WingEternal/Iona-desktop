﻿#ifndef IONADESKTOP_CORE_DISPLAY_MOVE_WIDGET_H
#define IONADESKTOP_CORE_DISPLAY_MOVE_WIDGET_H

#include <QWidget>
#include <QVariant>

namespace IonaDesktop {
namespace CoreDisplay {
    class MoveWidget : public QWidget
    {
        Q_OBJECT
    public:
        explicit MoveWidget(QWidget *parent);
        ~MoveWidget();

    private:
        QPixmap icon;
        QWidget* move_target;
        bool m_Lpressed;
        QPoint m_pos;

    protected:
        void paintEvent(QPaintEvent *ev);
        void mousePressEvent(QMouseEvent *ev);
        void mouseMoveEvent(QMouseEvent *ev);
        void mouseReleaseEvent(QMouseEvent *ev);

    signals:
        void setParam(const QString, const QVariant);
    };
}
}

#endif // IONADESKTOP_CORE_DISPLAY_MOVE_WIDGET_H
