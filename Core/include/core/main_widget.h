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
#include <QThread>
#include <QtMultimedia>
#include<QSystemTrayIcon>
#include <QtMultimedia/qmediaplayer.h>

#include "core/core_global.h"
#include "core/iona_widget.h"
#include "core/plugin_interface.h"

namespace IonaDesktop{
namespace Core {
    class MainWidget : public QWidget
    {
        Q_OBJECT
    public:
        explicit MainWidget(QWidget *parent = nullptr);
        ~MainWidget();
    private:
        IonaWidget *iona_widget_ptr;

        // Core Params
        QPoint main_window_posLT;

        // Tray-menu vars
        QSystemTrayIcon *tray_icon_ptr;
        int SetupTrayIcon();

    //    QMenu* m_right_click_menu;
        QPoint m_left_click_position;
        QElapsedTimer m_left_click_delay;
        bool m_left_pressed;

        // Json Config
        bool JsonArrayToIntArray(QJsonArray &json_array, int *array_ptr, int size);
        int SetupConfig();
        void SaveConfig();
        std::map<__int64, QJsonObject> M_config;

        // Plugin Handlers
        class PluginInstance
        {
        public:
            PluginInstance(QWidget* parent, QString path);
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
