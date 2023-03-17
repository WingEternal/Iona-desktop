#ifndef IONADESKTOP_CORE_DISPLAY_FSM_H
#define IONADESKTOP_CORE_DISPLAY_FSM_H

#include <QObject>
#include <QThread>
#include <QUrl>
#include <QMouseEvent>
#include <QMediaPlayer>
#include <QEasingCurve>

namespace IonaDesktop {
namespace CoreDisplay {
    class FSM : public QObject
    {
        Q_OBJECT
    public:
        explicit FSM(QObject* parent = nullptr);
        ~FSM();

        void register_ss();

    signals:
        void voiceLabelSetText(const QString str);
        void voicePlay(const QUrl media);

        void sendGLObjTask(const QString task_name, const QVariant target_state, const double interval, const QEasingCurve::Type curve);

        void dispatchLeftMouseButtonDown(const QPointF pt);
        void dispatchMouseMove(const QPointF pt);
        void dispatchLeftMouseButtonUp(const QPointF pt);

    public slots:
        void onStart();
        void mouseEventDispatch(const QEvent::Type type, const Qt::MouseButton b, const QPoint pt);
        void onPlayerStateChanged(const QMediaPlayer::PlaybackState state);
        void onGLObjTaskFinished(const QString& task_name);

    private:
        // Fsm
        double _fsm_rate;
        QTimer* _fsm_loop;

        double _fsm_clock;
        double _fsm_last_update_clock;
        QElapsedTimer* _fsm_elapsed;

        /**
         * 0x8000 _fsm_active
         * 0x4000 _voice_playing
         * 0x2000 _mouse_tracking
         * 0x1000 _range_mouse_tracking
        */
        uint16_t _buffer_states;
        enum FsmStatus {START, IDLE} _fsm_status;

    private slots:
        void loopStateMachine();
    };
}
}

#endif // IONADESKTOP_CORE_DISPLAY_FSM_H
