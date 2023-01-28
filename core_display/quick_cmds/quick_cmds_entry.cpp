#include "quick_cmds_entry.h"
#include "app_config.h"
#include <QPainter>
#include <QPaintEvent>

using namespace IonaDesktop::CoreDisplay;

QuickCmdsEntry::QuickCmdsEntry(QWidget *parent)
    : QWidget(parent)
{
    QPoint geo_pos;
    AppConfig::getInstance().getParam("/quick_cmds/pos", geo_pos);
    this->setGeometry(geo_pos.x(), geo_pos.y(), 50, 50);
    icon.load(":/icon/terminal.png");
}

QuickCmdsEntry::~QuickCmdsEntry()
{

}

void QuickCmdsEntry::paintEvent(QPaintEvent *ev)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, icon);
    ev->accept();
}

void QuickCmdsEntry::mousePressEvent(QMouseEvent *ev)
{

}

void QuickCmdsEntry::mouseMoveEvent(QMouseEvent *ev)
{

}

void QuickCmdsEntry::mouseReleaseEvent(QMouseEvent *ev)
{

}
