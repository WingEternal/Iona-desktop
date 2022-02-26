#ifndef IONADESKTOP_CORE_HITBOX_WIDGET_H
#define IONADESKTOP_CORE_HITBOX_WIDGET_H

#include "core/core_global.h"
#include <QWidget>
#include <QPaintEvent>

//#define DEBUG_HITBOX_RANGE
#ifdef DEBUG_HITBOX_RANGE
#include <QPainter>
#endif

/* This is a temporary file. May be abandoned when introducing Live2D. */

namespace IonaDesktop {
namespace Core {
    class HitboxWidget : public QWidget
    {
        Q_OBJECT
    public:
        explicit HitboxWidget(QWidget *parent = nullptr);
        ~HitboxWidget();
    private:

    protected:
        void paintEvent(QPaintEvent *ev);

    /* Hitbox Methods */
    protected:
        void mousePressEvent(QMouseEvent *ev);
    signals:
        void mouseClicked();
    };
}
}


#endif // HITBOX_WIDGET_H
