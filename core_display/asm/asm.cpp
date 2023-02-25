#include "app/app_msg_handler.h"
#include "asm/asm.h"
#include "voice/media_info.h"
#include <QDir>
#include <QTimer>
#include <QTime>

using namespace IonaDesktop::CoreDisplay;

ASM::ASM(QObject* parent)
    : QObject(parent),
      flag_active(false),
      flag_voice_playing(false)
{
    qDebug() << "[ASM] Constructed";
}

ASM::~ASM()
{

}

void ASM::register_ss()
{
    AppMsgHandler::getInstance().regSignal("/voice/label_text", this, SIGNAL(voiceLabelSetText(const QString)));
    AppMsgHandler::getInstance().regSignal("/voice/play", this, SIGNAL(voicePlay(const QUrl)));
    AppMsgHandler::getInstance().regSignal("/animate/ring_scale", this, SIGNAL(changeDataRingScale(const float)));
    AppMsgHandler::getInstance().regSignal("/animate/l2d_lbutton_down", this, SIGNAL(dispatchLeftMouseButtonDown(const QPointF)));
    AppMsgHandler::getInstance().regSignal("/animate/l2d_mouse_move", this, SIGNAL(dispatchMouseMove(const QPointF)));
    AppMsgHandler::getInstance().regSignal("/animate/l2d_lbutton_up", this, SIGNAL(dispatchLeftMouseButtonUp(const QPointF)));

    AppMsgHandler::getInstance().bindSlot("/voice/playback_state", this, SLOT(onPlayerStateChanged(const QMediaPlayer::PlaybackState)));
    AppMsgHandler::getInstance().bindSlot("/mouse", this, SLOT(mouseEventDispatch(const QEvent::Type, const Qt::MouseButton, const QPoint)));
}

void ASM::onStart()
{
    emit voiceLabelSetText(MediaInfoArray::start_info[0].text);
    emit voicePlay(QUrl::fromLocalFile(QDir::currentPath() + MediaInfoArray::start_info[0].file));
    
    asm_loop = new QTimer(this);
    connect(asm_loop, SIGNAL(timeout()), this, SLOT(LoopStateMachine()));
    asm_loop->start(200);

    flag_active = true;
    qDebug() << "[ASM] Now running on " << this->thread();
}

void ASM::mouseEventDispatch(const QEvent::Type type, const Qt::MouseButton b, const QPoint pt)
{
    if(!flag_active) return;
    if(b == Qt::LeftButton)
    {
        switch(type)
        {
        case QEvent::MouseButtonPress:
            emit dispatchLeftMouseButtonDown(pt);
            break;
        case QEvent::MouseMove:
            emit dispatchMouseMove(pt);
            break;
        case QEvent::MouseButtonRelease:
            emit dispatchLeftMouseButtonUp(pt);
            break;
        default:
            break;
        }
    }
}

void ASM::onPlayerStateChanged(const QMediaPlayer::PlaybackState state)
{
    if(state == QMediaPlayer::PlayingState)
        flag_voice_playing = true;
//    if(state == QMediaPlayer::StoppedState)
    else
        flag_voice_playing = false;
}

void ASM::LoopStateMachine()
{
    // Check chime
    QTime curr_time = QTime::currentTime();
    if(curr_time.minute() == 0 && curr_time.second() == 0
        && !flag_voice_playing)
    {
        emit voicePlay(QUrl::fromLocalFile(QDir::currentPath() + MediaInfoArray::chime_info[curr_time.hour()].file));
        emit voiceLabelSetText(MediaInfoArray::chime_info[curr_time.hour()].text);
    }
}
