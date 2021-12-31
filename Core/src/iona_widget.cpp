#include "core/iona_widget.h"
#include "ui_iona_widget.h"

using namespace IonaDesktop::Core;

IonaWidget::IonaWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IonaWidget)
{
    ui->setupUi(this);
    setGeometry(210, 10, 600, 500);
    voice_on_time_checker_ptr = new QTimer(this);
    voice_on_time_checker_ptr->setInterval(500);
    connect(voice_on_time_checker_ptr, SIGNAL(timeout()), this, SLOT(Slot_OnTimeChecker_Timeout()));
    voice_on_time_checker_ptr->start();

    voice_player_ptr = new QMediaPlayer(this);
    voice_player_ptr->setMedia(QUrl("qrc:/charater/voice/Iona01.mp3"));
    voice_player_ptr->setVolume(50);
    voice_player_ptr->play();
}

IonaWidget::~IonaWidget()
{
    delete ui;
    delete voice_on_time_checker_ptr;
    delete voice_player_ptr;
}

void IonaWidget::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "Judge: single click";
    if(voice_player_ptr->state() != QMediaPlayer::StoppedState)
        voice_player_ptr->stop();
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    int randVoice = qrand()%2 + 1;
    switch(randVoice)
    {
    case 1:
        voice_player_ptr->setMedia(QUrl("qrc:/charater/voice/Iona02.mp3"));
        break;
    case 2:
        voice_player_ptr->setMedia(QUrl("qrc:/charater/voice/Iona03.mp3"));
        break;
    case 3:
        voice_player_ptr->setMedia(QUrl("qrc:/charater/voice/Iona04.mp3"));
        break;
    }
    voice_player_ptr->play();
    event->accept();
}

void IonaWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap pixmap, pixmapScaled;
    pixmap.load(":/charater/image/Iona.png");
    pixmapScaled = pixmap.scaled(600, 500, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    painter.drawPixmap(0,0,pixmapScaled);
    setFixedSize(pixmapScaled.size());
    setMask(pixmapScaled.mask());
    event->accept();
}

void IonaWidget::Slot_OnTimeChecker_Timeout()
{
    QTime currTime = QTime::currentTime();
//    qDebug() << "QTimer voiceOnTime interval TRIG: " << currTime;
    if(voice_player_ptr->state() == QMediaPlayer::StoppedState && currTime.minute() == 0 && currTime.second() == 0)
    {
        if(currTime.hour() < 10)
        {
            QString voiceFileString = "qrc:/charater/voice/0" + QString::number(currTime.hour()) + "00.mp3";
            voice_player_ptr->setMedia(QUrl(voiceFileString));
            voice_player_ptr->play();
        }
        else
        {
            QString voiceFileString = "qrc:/charater/voice/" + QString::number(currTime.hour()) + "00.mp3";
            voice_player_ptr->setMedia(QUrl(voiceFileString));
            voice_player_ptr->play();
        }
    }
}
