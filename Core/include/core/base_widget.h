#ifndef IONADESKTOP_CORE_BASE_WIDGET_H
#define IONADESKTOP_CORE_BASE_WIDGET_H

#include "core/core_global.h"
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>

/* Description:
* Base widget of all other widgets */

/* DISPLAY INCLUDES */
#include "core/gl_widget.h"
/* MASK INCLUDES*/
#include <QPixmap>
#include <QBitmap>
/* VOICE INCLUDES */
#include <QLabel>
#include <QtMultimedia>
#include <QtMultimedia/qmediaplayer.h>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
/* TRAY MENU INCLUDES */
#include <QSystemTrayIcon>
#include <QMenu>
/* JSON INCLUDES */
#include <map>
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
/* PLUGIN INCLUDES */
#include "core/plugin_interface.h"
#include "core/plugin_instance.h"
/* MOVE_WIDGET INCLUDES */
#include "core/move_widget.h"
/* HITBOX_WIDGET INCLUDES */
#include "core/hitbox_widget.h"

namespace IonaDesktop {
namespace Core {
    class BaseWidget : public QWidget
    {
        Q_OBJECT
    public:
        explicit BaseWidget(QWidget *parent = nullptr);
        ~BaseWidget();
    protected:
         QPoint window_global_posLT;
        void paintEvent(QPaintEvent *ev);

        /* Main Display */
    private:
        void setupGLWidget();
        GLWidget *gl_widget_ptr;

        /* Mask */
    private:
        void setupMask();

        /* Voice MediaPlayer & Label */
    private:
        void setupVoice();
        QTimer *voice_timer_ptr;
        QMediaPlayer *voice_player_ptr;
        QLabel *voice_label_ptr;
        QGraphicsOpacityEffect *label_opacity_fx;
        QPropertyAnimation *label_animation;
        const int label_animation_duration = 100;
    private slots:
        void Slot_Hitbox_Hit();
        void Slot_VoiceTimer_Timeout();
        void Slot_Player_StateChanged(QMediaPlayer::State state);
        void Slot_Animation_Finished();

        /* Tray Menu */
    private:
        void setupTrayIcon();
        QSystemTrayIcon *tray_icon_ptr;
    private slots:
        void Slot_TrayIcon_Activated(QSystemTrayIcon::ActivationReason reason);
        void Slot_TrayMenu_Hide();
        void Slot_TrayMenu_ResetGeometry();
        void Slot_TrayMenu_Exit();

        /* Json Config */
    private:
        int setupConfig();
        void saveConfig();
        std::map<int32_t, QJsonObject> M_config;

        /* Plugins */
    private:
        int setupPlugins();
        std::map<int32_t, QSharedPointer<PluginInstance>> M_plugins;

        /* Move_widget */
    private:
        void setupMoveWidget();
        MoveWidget *move_widget_ptr;

        /* Hitbox_widget */
    private:
        void setupHitboxWidgets();
        HitboxWidget *head_hitbox_widget;
        HitboxWidget *body_hitbox_widget;
    };
}
}

#endif // IONADESKTOP_CORE_BASE_WIDGET_H
