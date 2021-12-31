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

#include "core/core_global.h"

namespace Ui {
class IonaWidget;
}
namespace IonaDesktop{
namespace Core {
    class IonaWidget : public QWidget
    {
        Q_OBJECT

    public:
        explicit IonaWidget(QWidget *parent = nullptr);
        ~IonaWidget();

    private:
        Ui::IonaWidget *ui;

        QTimer *voice_on_time_checker_ptr;
        QMediaPlayer *voice_player_ptr;

    protected:
        void mousePressEvent(QMouseEvent* event);
        void paintEvent(QPaintEvent* event);

    private slots:
        void Slot_OnTimeChecker_Timeout();
    };
}
}
#endif // IONADESKTOP_CORE_IONA_WIDGET_H
