#ifndef IONA_WIDGET_H
#define IONA_WIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QTimer>
#include <QPainter>
#include <QBitmap>
#include <QPixmap>
#include <QMenu>
#include <QtMultimedia>
#include <QtMultimedia/qmediaplayer.h>

namespace Ui {
class IonaWidget;
}

class IonaWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IonaWidget(QWidget *parent = nullptr);
    ~IonaWidget();

private:
    Ui::IonaWidget *ui;

    QTimer* voice_on_time_checker;
    QMediaPlayer* voice_player;

protected:
    void mousePressEvent(QMouseEvent* event);
    void paintEvent(QPaintEvent* event);
public slots:
    void voiceOnTime_timeout();
};

#endif // IONA_WIDGET_H
