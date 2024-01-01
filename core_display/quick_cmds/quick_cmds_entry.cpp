#include "quick_cmds/quick_cmds_entry.h"

#include <QPaintEvent>
#include <QPainter>

using namespace IonaDesktop::CoreDisplay;

QuickCmdsEntry::QuickCmdsEntry(QWidget *parent) : QWidget(parent) {
  this->setGeometry(0.75 * parent->width(), 0.6 * parent->height(), 50, 50);
  icon.load(":/icon/terminal.png");
}

QuickCmdsEntry::~QuickCmdsEntry() {}

void QuickCmdsEntry::paintEvent(QPaintEvent *ev) {
  QPainter painter(this);
  painter.drawPixmap(0, 0, icon);
  ev->accept();
}

void QuickCmdsEntry::mousePressEvent(QMouseEvent *ev) {}

void QuickCmdsEntry::mouseMoveEvent(QMouseEvent *ev) {}

void QuickCmdsEntry::mouseReleaseEvent(QMouseEvent *ev) {}
