#include "voice/voice_label.h"
#include "app/app_msg_handler.h"

using namespace IonaDesktop::CoreDisplay;

VoiceLabel::VoiceLabel(QWidget *parent) : QLabel(parent)
{
    this->setStyleSheet("font-family:微软雅黑; font-size:16px; color: black; background-color:rgba(255, 255, 255, 180); border-radius:15px;");
    this->setWordWrap(true);
    this->setAlignment(Qt::AlignCenter);
    this->setGeometry(250, 265, 220, 60);

    // Animation effects
    label_opacity_fx = new QGraphicsOpacityEffect(this);
    this->setGraphicsEffect(label_opacity_fx);
    this->setWindowOpacity(0);
    label_animation = new QPropertyAnimation(label_opacity_fx, "opacity", this);
    label_animation->setDuration(label_animate_duration);
    label_animation->setStartValue(0);
    label_animation->setEndValue(1);
    label_animation->setEasingCurve(QEasingCurve::Linear);

    AppMsgHandler::getInstance().bindSlot("/voice/playback_state", this, SLOT(onPlayerPlaybackStateChanged(QMediaPlayer::PlaybackState)));
    AppMsgHandler::getInstance().bindSlot("/voice/label_text", this, SLOT(setText(const QString&)));

    connect(label_animation, SIGNAL(finished()), 
        this, SLOT(onAnimationFinished()));
}

VoiceLabel::~VoiceLabel()
{

}

void VoiceLabel::onPlayerPlaybackStateChanged(QMediaPlayer::PlaybackState state)
{
    if(state == QMediaPlayer::PlayingState)
    {
        this->show();
        label_animation->setDirection(QAbstractAnimation::Forward);
        label_animation->start();
    }
    else if(state == QMediaPlayer::StoppedState)
    {
        label_animation->setDirection(QAbstractAnimation::Backward);
        label_animation->start();
    }
}

void VoiceLabel::onAnimationFinished()
{
    if(label_animation->direction() == QAbstractAnimation::Backward)
    {
        this->hide();
    }
}
