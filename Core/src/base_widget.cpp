#include "core/base_widget.h"
#include "hook.h"

using namespace IonaDesktop::Core;

BaseWidget* SingletonWarpper::instance = nullptr;

BaseWidget* SingletonWarpper::getInstance()
{
    if(instance == nullptr)
        instance = new BaseWidget(nullptr);
    return instance;
}

void SingletonWarpper::releaseInstance()
{
    if(instance)
        delete instance;
}

BaseWidget::BaseWidget(QWidget *parent)
    : QWidget (parent)
{
    setupConfig();
    setupTrayIcon();

    this->setWindowFlag(Qt::Tool);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlag(Qt::WindowStaysOnTopHint);
    this->setWindowFlag(Qt::NoDropShadowWindowHint);
    setGeometry(window_global_posLT.x(), window_global_posLT.y(), 600, 500);
//    setupMask();

    setupGLWidget();
    setupMoveWidget();
    setupVoice();
    setupHitboxWidgets();

    // Note: Do NOT install hook before setting window attributes
    installHook();
}

BaseWidget::~BaseWidget()
{
    uninstallHook();
}

void BaseWidget::paintEvent(QPaintEvent *ev)
{
    ev->accept();
}

int BaseWidget::installHook()
{
#ifdef Q_OS_WIN
//    HMODULE hDll = nullptr;
    HWND m_hWnd = (HWND)this->winId();
    BOOL hookMouseState = Hook::InstallMouseHook(m_hWnd);
    BOOL hookKeybordState = Hook::InstallKeybordHook();
    if (!hookMouseState && !hookKeybordState)
        qDebug() << "hook failed!";
    else
        qDebug() << "hook success!";
#endif
    return 0;
}

int BaseWidget::uninstallHook()
{
#ifdef Q_OS_WIN
    BOOL hookMouseState = Hook::UnInstallMouseHook();
    BOOL hookKeybordState = Hook::UnInstallKeybordHook();
    if (!hookMouseState && !hookKeybordState)
        qDebug() << "unhook failed!";
    else
        qDebug() << "unhook success!";
#endif
    return 0;
}
