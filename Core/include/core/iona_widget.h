#ifndef IONADESKTOP_CORE_IONA_WIDGET_H
#define IONADESKTOP_CORE_IONA_WIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QTimer>
#include <QPainter>
#include <QBitmap>
#include <QPixmap>
#include <QMenu>
#include <QtMultimedia>
#include <QtMultimedia/qmediaplayer.h>

#include "core/global_definitions.h"

namespace Ui {
class IonaWidget;
}
namespace IonaDesktop{
namespace Core {
    class IonaWidget : public QWidget
    {
        Q_OBJECT

    public:
        explicit IonaWidget(QSize &paint_scale, QWidget *parent = nullptr);
        ~IonaWidget();

    private:
        Ui::IonaWidget *ui;
        QSize iona_paint_scale;

        QTimer* voice_on_time_checker;
        QMediaPlayer* voice_player;

    protected:
        void mousePressEvent(QMouseEvent* event);
        void paintEvent(QPaintEvent* event);

    public slots:
        void Slot_OnTimeChecker_Timeout();
    };
}
}
#endif // IONADESKTOP_CORE_IONA_WIDGET_H
