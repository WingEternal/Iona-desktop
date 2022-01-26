#include "core/base_widget.h"

using namespace IonaDesktop::Core;

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

    setupGLWidget();
    setupPlugins();
    setupMoveWidget();
    setupVoice();
}

BaseWidget::~BaseWidget()
{

}

void BaseWidget::paintEvent(QPaintEvent *ev)
{
    ev->accept();
}
