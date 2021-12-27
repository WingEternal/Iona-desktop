#include "main_widget/iona_widget.h"
#include "ui_iona_widget.h"

IonaWidget::IonaWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IonaWidget)
{
    ui->setupUi(this);
    voice_on_time_checker = new QTimer(this);
    voice_on_time_checker->setInterval(500);
    connect(voice_on_time_checker, SIGNAL(timeout()), this, SLOT(voiceOnTime_timeout()));
    voice_on_time_checker->start();

    voice_player = new QMediaPlayer(this);
    voice_player->setMedia(QUrl("qrc:/charater/voice/Iona01.mp3"));
    voice_player->setVolume(50);
    voice_player->play();
}

IonaWidget::~IonaWidget()
{
    delete ui;
    delete voice_on_time_checker;
    delete voice_player;
}

void IonaWidget::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "Judge: single click";
    if(voice_player->state() != QMediaPlayer::StoppedState)
        voice_player->stop();
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    int randVoice = qrand()%2 + 1;
    switch(randVoice)
    {
    case 1:
        voice_player->setMedia(QUrl("qrc:/charater/voice/Iona02.mp3"));
        break;
    case 2:
        voice_player->setMedia(QUrl("qrc:/charater/voice/Iona03.mp3"));
        break;
    case 3:
        voice_player->setMedia(QUrl("qrc:/charater/voice/Iona04.mp3"));
        break;
    }
    voice_player->play();
}

void IonaWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap pixmap, pixmapScaled;
    pixmap.load(":/charater/image/Iona.png");
    pixmapScaled = pixmap.scaled(QSize(600,500), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    painter.drawPixmap(0,0,pixmapScaled);
    setFixedSize(pixmapScaled.size());
    setMask(pixmapScaled.mask());
    event->accept();
}

void IonaWidget::voiceOnTime_timeout()
{
    QTime currTime = QTime::currentTime();
//    qDebug() << "QTimer voiceOnTime interval TRIG: " << currTime;
    if(voice_player->state() == QMediaPlayer::StoppedState && currTime.minute() == 0 && currTime.second() == 0)
    {
        if(currTime.hour() < 10)
        {
            QString voiceFileString = "qrc:/charater/voice/0" + QString::number(currTime.hour()) + "00.mp3";
            voice_player->setMedia(QUrl(voiceFileString));
            voice_player->play();
        }
        else
        {
            QString voiceFileString = "qrc:/charater/voice/" + QString::number(currTime.hour()) + "00.mp3";
            voice_player->setMedia(QUrl(voiceFileString));
            voice_player->play();
        }
    }
}
