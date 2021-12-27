#ifndef MAIN_WIDGET_H
#define MAIN_WIDGET_H

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

#include "main_widget/iona_widget.h"

namespace Ui {
    class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private:
    Ui::MainWidget *ui;
    IonaWidget *iona_widget_ptr;

    QMenu* trayMenu;
    QSystemTrayIcon* trayIcon;
    QAction* trayShowWindow;
    QAction* trayExit;

    QMenu* m_right_click_menu;
    QPoint m_left_click_position;
    QElapsedTimer m_left_click_delay;
    bool m_left_pressed;

protected:
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void paintEvent(QPaintEvent* event);
public slots:
    void trayIcon_activated(QSystemTrayIcon::ActivationReason reason);
    void trayMenu_exit();
};

#endif // MAIN_WIDGET_H
