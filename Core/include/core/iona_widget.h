#ifndef IONADESKTOP_CORE_IONA_WIDGET_H
#define IONADESKTOP_CORE_IONA_WIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QTimer>
#include <QLabel>
#include <QtMultimedia>
#include <QtMultimedia/qmediaplayer.h>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

#include "core/core_global.h"
#include "core/portrait_widget.h"

namespace IonaDesktop{
namespace Core {
    // A empty (abstract) widget, used to contain all component of Iona
    class IonaWidget : public QWidget
    {
        Q_OBJECT
    public:
        explicit IonaWidget(QWidget *parent = nullptr);
        ~IonaWidget();
    private:
        PortraitWidget *portrait_widget_ptr;
        QTimer voice_timer;
        QMediaPlayer voice_player;
        QLabel voice_label;
        QGraphicsOpacityEffect *label_opacity_fx;
        QPropertyAnimation *label_animation;
        const int label_animation_duration;
    protected:
//        void mousePressEvent(QMouseEvent* event);
        void paintEvent(QPaintEvent* event);
    private slots:
        void Slot_Portrait_MousePress();
        void Slot_VoiceTimer_Timeout();
        void Slot_Player_StateChanged(QMediaPlayer::State state);
        void Slot_Animation_Finished();
    };
}
}
#endif // IONADESKTOP_CORE_IONA_WIDGET_H
