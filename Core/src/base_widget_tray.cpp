#include "core/base_widget.h"

using namespace IonaDesktop::Core;

void BaseWidget::setupTrayIcon()
{
    tray_icon_ptr = new QSystemTrayIcon(this);
    QIcon icon = QIcon(":/icon/icon_anchor.png");
    tray_icon_ptr->setIcon(icon);
    tray_icon_ptr->setToolTip(QString("Iona-desktop"));
    connect(tray_icon_ptr, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(Slot_TrayIcon_Activated(QSystemTrayIcon::ActivationReason)));
    tray_icon_ptr->show();
    QMenu *tray_menu_ptr = new QMenu();
    tray_icon_ptr->setContextMenu(tray_menu_ptr);

    QAction *act_tray_hide_ptr = new QAction(QString("Hide"));
    act_tray_hide_ptr->setCheckable(true);
    act_tray_hide_ptr->setChecked(false);
    connect(act_tray_hide_ptr, SIGNAL(triggered()), this, SLOT(Slot_TrayMenu_Hide()));
    tray_menu_ptr->addAction(act_tray_hide_ptr);

    QAction *act_tray_reset_geometry_ptr = new QAction(QString("Reset Geometry"));
    connect(act_tray_reset_geometry_ptr, SIGNAL(triggered()), this, SLOT(Slot_TrayMenu_ResetGeometry()));
    tray_menu_ptr->addAction(act_tray_reset_geometry_ptr);

    QAction *act_tray_exit_ptr = new QAction(QString("Exit"));
    connect(act_tray_exit_ptr, SIGNAL(triggered()), this, SLOT(Slot_TrayMenu_Exit()));
    tray_menu_ptr->addAction(act_tray_exit_ptr);
}

void BaseWidget::Slot_TrayMenu_Hide()
{
    if(this->isHidden()) show();
    else hide();
}

void BaseWidget::Slot_TrayIcon_Activated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
    case QSystemTrayIcon::Trigger:
        break;
//    case QSystemTrayIcon::DoubleClick:
    default:
        break;
    }
}

void BaseWidget::Slot_TrayMenu_ResetGeometry()
{
    window_global_posLT = default_main_window_posLT;
    move(window_global_posLT.x(), window_global_posLT.y());
    update();
}

void BaseWidget::Slot_TrayMenu_Exit()
{
    saveConfig();
    exit(0);
}
