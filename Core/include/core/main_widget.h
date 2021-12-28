#ifndef IONADESKTOP_CORE_MAIN_WIDGET_H
#define IONADESKTOP_CORE_MAIN_WIDGET_H

#include <map>
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include <QWidget>
#include <QMouseEvent>
#include <QElapsedTimer>
#include <QTimer>
#include <QPainter>
#include <QBitmap>
#include <QPixmap>
#include <QMenu>
#include <QtMultimedia>
#include<QSystemTrayIcon>
#include <QtMultimedia/qmediaplayer.h>

#include "core/global_definitions.h"
#include "core/iona_widget.h"
#include "core/plugin_interface.h"
namespace Ui {
    class MainWidget;
}
namespace IonaDesktop{
namespace Core {
    class MainWidget : public QWidget
    {
        Q_OBJECT
    public:
        explicit MainWidget(QWidget *parent = nullptr);
        ~MainWidget();
    private:
        Ui::MainWidget *ui;
        IonaWidget *iona_widget_ptr;

        // Core Params
        QRect main_window_geometry;
        QSize I401_paint_scale;
        QPoint iona_window_position;
        QSize iona_paint_scale;

        // Tray-menu vars
        QSystemTrayIcon *tray_icon_ptr;
//        QMenu *tray_menu_ptr;
//        QAction *act_tray_reset_location_ptr;
//        QAction *act_tray_exit_ptr;
        int SetupTrayIcon();

    //    QMenu* m_right_click_menu;
        QPoint m_left_click_position;
        QElapsedTimer m_left_click_delay;
        bool m_left_pressed;

        // Json Config
        int SetupConfig();
        std::map<__int64, QJsonObject> M_config;
        void SaveConfig();

        // Plugin Handlers
        class PluginInstance
        {
        public:
            PluginInstance(QObject* parent, QString path);
            ~PluginInstance();
            inline PluginBase* getHandler()
            { return plugin_handler; }
        private:
            QSharedPointer<QPluginLoader> loader_ptr;
            PluginBase* plugin_handler;
        };
        int SetupPlugins();
        std::map<__int64, QSharedPointer<PluginInstance>> M_plugins;

    protected:
        void mouseMoveEvent(QMouseEvent* event);
        void mousePressEvent(QMouseEvent* event);
        void mouseReleaseEvent(QMouseEvent* event);
        void paintEvent(QPaintEvent* event);

    private slots:
        // Tray-menu slots
        void Slot_TrayIcon_Activated(QSystemTrayIcon::ActivationReason reason);
        void Slot_TrayMenu_ResetGeometry();
        void Slot_TrayMenu_Exit();
    };
}
}
#endif // IONADESKTOP_CORE_MAIN_WIDGET_H
