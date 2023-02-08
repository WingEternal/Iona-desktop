#ifndef IONA_DESKTOP_CORE_DISPLAY_VOICE_LABEL_H
#define IONA_DESKTOP_CORE_DISPLAY_VOICE_LABEL_H

#include <QLabel>
#include <QtMultimedia>
#include <QtMultimedia/qmediaplayer.h>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QAudioOutput>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

namespace IonaDesktop {
namespace CoreDisplay {
    class VoiceLabel : public QLabel
    {
        Q_OBJECT
    public:
        explicit VoiceLabel(QWidget *parent = nullptr);
        ~VoiceLabel();
    private:
        QTimer* voice_timer;
        QMediaPlayer* voice_player;
        QAudioOutput* audio_output;
        
        QThread* media_thrd;
        QGraphicsOpacityEffect* label_opacity_fx;
        QPropertyAnimation* label_animation;
        const int label_animate_duration = 100;

    signals:

    public slots:
        void playRandomVoice();

    private slots:
        void checkChime();
        void changeLabelVisibilty(QMediaPlayer::PlaybackState);
        void onAnimationFinished();
    };
}
}

#endif // IONA_DESKTOP_CORE_DISPLAY_VOICE_LABEL_H
