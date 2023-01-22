#include "tray.h"
#include "app_msg_handler.h"
#include <QMenu>
#include <QAction>
#include <QCoreApplication>

using namespace IonaDesktop::CoreDisplay;

Tray::Tray(QWidget* parent)
    : QSystemTrayIcon(parent)
{
    QIcon icon(":/icon/icon_anchor.png");
    this->setIcon(icon);
    this->setToolTip(QString("Iona-desktop"));
    connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayActivated(const QSystemTrayIcon::ActivationReason)));
    this->show();
    menu = new QMenu();
    this->setContextMenu(menu);

    app_hide = new QAction(QString("hide"));
    app_hide->setCheckable(true);
    app_hide->setChecked(false);
    AppMsgHandler::getInstance().regSignal
        ("/window/hide", app_hide, SIGNAL(triggered()));
    menu->addAction(app_hide);

    app_reset_geo = new QAction(QString("reset geometry"));
    AppMsgHandler::getInstance().regSignal
        ("/window/reset_geometry", app_reset_geo, SIGNAL(triggered()));
    menu->addAction(app_reset_geo);

    app_exit = new QAction(QString("exit"));
    connect(app_exit, SIGNAL(triggered()), this, SLOT(terminate()));
//    AppMsgHandler::getInstance().regSignal
//        ("/app/setting_sync", app_exit, SIGNAL(triggered()));
    menu->addAction(app_exit);
}

void Tray::terminate()
{
    QCoreApplication::exit(0);
}

void Tray::trayActivated(const QSystemTrayIcon::ActivationReason& reason)
{
    Q_UNUSED(reason);
}
