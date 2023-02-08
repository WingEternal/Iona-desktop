#include "app/app_widget_container.h"
#include "app/app_config.h"
#include "app/app_msg_handle.h"

using namespace IonaDesktop::CoreDisplay;

AppWidgetContainer::AppWidgetContainer(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowFlag(Qt::Tool);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlag(Qt::WindowStaysOnTopHint);
    this->setWindowFlag(Qt::NoDropShadowWindowHint);
    QRect geo;
    AppConfig::getInstance().getParam("/window/size", geo);
    this->setGeometry(geo);

    AppMsgHandler::getInstance().bindSlot
        ("/window/hide", this, SLOT(trayRequestHide()));
    AppMsgHandler::getInstance().bindSlot
        ("/window/reset_geometry", this,  SLOT(trayRequestResetGeometry()));

    tray = new Tray(this);

    move_ctrl = new MoveWidget(this);
    move_ctrl->show();

    quick_cmds = new QuickCmdsEntry(this);
    quick_cmds->show();

    voice_label = new VoiceLabel(this);
    voice_label->raise();
    voice_label->show();

    gl_widget = new GLWidget(this);
    gl_widget->lower();
    gl_widget->show();

    installHook();
}

AppWidgetContainer::~AppWidgetContainer()
{
    uninstallHook();
}

void AppWidgetContainer::paintEvent(QPaintEvent* ev)
{
    ev->accept();
}

void AppWidgetContainer::trayRequestHide()
{
    if(this->isHidden())
        this->show();
    else this->hide();
}

void AppWidgetContainer::trayRequestResetGeometry()
{
    QRect geo;
    AppConfig::getInstance().getParam("/window/size", geo);
    this->setGeometry(0, 0, geo.width(), geo.height());
    update();
}

#ifdef Q_OS_WIN
bool AppWidgetContainer::installHook()
{
//    HMODULE hDll = nullptr;
    HWND m_hWnd = (HWND)this->winId();
    BOOL hookMouseState = Hook::InstallMouseHook(m_hWnd);
//    BOOL hookKeybordState = Hook::InstallKeybordHook();
    if (!hookMouseState)
        qDebug() << "hook failed!";
    else
        qDebug() << "hook success!";
    return hookMouseState;
}

bool AppWidgetContainer::uninstallHook()
{
    BOOL hookMouseState = Hook::UnInstallMouseHook();
//    BOOL hookKeybordState = Hook::UnInstallKeybordHook();
    if (!hookMouseState)
        qDebug() << "unhook failed!";
    else
        qDebug() << "unhook success!";
    return hookMouseState;
}
#endif
