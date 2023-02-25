#ifndef IONADESKTOP_CORE_DISPLAY_ASM_H
#define IONADESKTOP_CORE_DISPLAY_ASM_H

#include <QObject>
#include <QThread>
#include <QUrl>
#include <QMouseEvent>
#include <QMediaPlayer>

namespace IonaDesktop {
namespace CoreDisplay {
    class ASM : public QObject
    {
        Q_OBJECT
    public:
        explicit ASM(QObject* parent = nullptr);
        ~ASM();

        void register_ss();

    private:
        QTimer* asm_loop;
        bool flag_active;
        bool flag_voice_playing;

    signals:
        void voiceLabelSetText(const QString str);
        void voicePlay(const QUrl media);

        void changeDataRingScale(const float scale);
        void dispatchLeftMouseButtonDown(const QPointF pt);
        void dispatchMouseMove(const QPointF pt);
        void dispatchLeftMouseButtonUp(const QPointF pt);

    public slots:
        void onStart();
        void mouseEventDispatch(const QEvent::Type type, const Qt::MouseButton b, const QPoint pt);
        void onPlayerStateChanged(const QMediaPlayer::PlaybackState state);

    private slots:
        void LoopStateMachine();
    };
}
}

#endif // IONADESKTOP_CORE_DISPLAY_ASM_H
