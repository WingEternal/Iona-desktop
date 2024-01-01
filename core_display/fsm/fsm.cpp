#include "fsm/fsm.h"

#include <QDir>
#include <QEasingCurve>
#include <QElapsedTimer>
#include <QMetaMethod>
#include <QTime>
#include <QTimer>

#include "app/app_msg_handler.h"
#include "app/app_param.h"
#include "voice/media_info.h"

using namespace IonaDesktop::CoreDisplay;

FSM::FSM(QObject* parent)
    : QObject(parent),
      _fsm_rate(30),
      _fsm_clock(0.0),
      _fsm_last_update_clock(0.0),
      _fsm_status(START) {
  _buffer_states = 0x0;
  qDebug() << "[FSM] Constructed";
}

FSM::~FSM() {}

void FSM::register_ss() {
  AppMsgHandler& handler = AppMsgHandler::getInstance();

  handler.regSignal("/voice/label_text", this,
                    SIGNAL(voiceLabelSetText(const QString)));
  handler.regSignal("/voice/play", this, SIGNAL(voicePlay(const QUrl)));
  handler.bindSlot(
      "/voice/playback_state", this,
      SLOT(onPlayerStateChanged(const QMediaPlayer::PlaybackState)));

  handler.regSignal(
      "/animate/task", this,
      SIGNAL(sendGLObjTask(const QString, const QVariant, const double,
                           const QEasingCurve::Type)));
  handler.bindSlot("/animate/task/finished", this,
                   SLOT(onGLObjTaskFinished(const QString&)));

  handler.regSignal("/animate/l2d_lbutton_down", this,
                    SIGNAL(dispatchLeftMouseButtonDown(const QPointF)));
  handler.regSignal("/animate/l2d_mouse_move", this,
                    SIGNAL(dispatchMouseMove(const QPointF)));
  handler.regSignal("/animate/l2d_lbutton_up", this,
                    SIGNAL(dispatchLeftMouseButtonUp(const QPointF)));
  handler.bindSlot(
      "/mouse", this,
      SLOT(mouseEventDispatch(const QEvent::Type, const Qt::MouseButton,
                              const QPoint)));
}

void FSM::onStart() {
  _fsm_loop = new QTimer(this);
  _fsm_loop->setTimerType(Qt::PreciseTimer);
  AppParam::getInstance().getParam("S$animate/update_rate", _fsm_rate);
  _fsm_rate = qRound(1000.0 / _fsm_rate);
  connect(_fsm_loop, SIGNAL(timeout()), this, SLOT(loopStateMachine()));
  AppMsgHandler::getInstance().regSignal("/animate/update", _fsm_loop,
                                         SIGNAL(timeout()));
  _fsm_elapsed = new QElapsedTimer;
  AppParam::getInstance().setParam("D$clock", _fsm_clock);
  AppParam::getInstance().setParam(
      "D$clock/delta", (_fsm_clock - _fsm_last_update_clock) / 1000.0);
  qDebug() << "[FSM] Now running on " << this->thread();

  _fsm_loop->start(_fsm_rate);
  _fsm_elapsed->start();

  _buffer_states |= 0x8000;
}

void FSM::mouseEventDispatch(const QEvent::Type type, const Qt::MouseButton b,
                             const QPoint pt) {
  if (!(_buffer_states & 0x8000) || _fsm_status != IDLE) return;
  if (b == Qt::LeftButton) {
    QPoint widget_center;
    AppParam::getInstance().getParam("D$window/center", widget_center);
    static constexpr double focus_px_range = 800;
    //        static constexpr double focus_px_range_square = focus_px_range *
    //        focus_px_range; if(pow(pt.x() - widget_center.x(), 2) + pow(pt.y()
    //        - widget_center.y(), 2) <= focus_px_range_square)
    if ((pt - widget_center).manhattanLength() < focus_px_range) {
      if (!(_buffer_states & 0x1000)) {
        emit dispatchLeftMouseButtonDown(pt);
        _buffer_states |= 0x1000;
      } else
        emit dispatchMouseMove(pt);
    } else if (_buffer_states & 0x1000) {
      _buffer_states &= ~0x1000;
      if (!(_buffer_states & 0x2000)) emit dispatchLeftMouseButtonUp(pt);
    }
    switch (type) {
      case QEvent::MouseButtonPress:
        emit dispatchLeftMouseButtonDown(pt);
        _buffer_states |= 0x2000;
        break;
      case QEvent::MouseMove:
        emit dispatchMouseMove(pt);
        break;
      case QEvent::MouseButtonRelease:
        emit dispatchLeftMouseButtonUp(pt);
        _buffer_states &= ~0x2000;
        break;
      default:
        break;
    }
  }
}

void FSM::onPlayerStateChanged(const QMediaPlayer::PlaybackState state) {
  if (state == QMediaPlayer::PlayingState) _buffer_states |= 0x4000;
  //    if(state == QMediaPlayer::StoppedState)
  else
    _buffer_states &= ~0x4000;
}

void FSM::loopStateMachine() {
  // Update elapsed time
  _fsm_clock = _fsm_elapsed->elapsed();
  AppParam::getInstance().setParam("D$clock", _fsm_elapsed->elapsed());
  AppParam::getInstance().setParam(
      "D$clock/delta", (_fsm_clock - _fsm_last_update_clock) / 1000.0);
  _fsm_last_update_clock = _fsm_clock;

  // On start
  switch (_fsm_status) {
    case START: {
      // Check whether signals are connected
      if (!(_buffer_states & 0x1) &&
          this->isSignalConnected(
              QMetaMethod::fromSignal(&FSM::voiceLabelSetText)) &&
          this->isSignalConnected(QMetaMethod::fromSignal(&FSM::voicePlay))) {
        emit voiceLabelSetText(MediaInfoArray::start_info[0].text);
        emit voicePlay(QUrl::fromLocalFile(QDir::currentPath() +
                                           MediaInfoArray::start_info[0].file));
        _buffer_states |= 0x1;
      }
      if (!(_buffer_states & 0x2) &&
          this->isSignalConnected(
              QMetaMethod::fromSignal(&FSM::dispatchLeftMouseButtonDown)) &&
          this->isSignalConnected(
              QMetaMethod::fromSignal(&FSM::dispatchMouseMove)) &&
          this->isSignalConnected(
              QMetaMethod::fromSignal(&FSM::dispatchLeftMouseButtonUp)))
        _buffer_states |= 0x2;
      if (!(_buffer_states & 0x4) &&
          this->isSignalConnected(
              QMetaMethod::fromSignal(&FSM::sendGLObjTask))) {
        emit sendGLObjTask("DRAStartMotion", 0, 0, QEasingCurve::Linear);
        _buffer_states |= 0x4;
      }
      break;
    }
    case IDLE: {
      break;
    }
    default:
      break;
  }

  // Check chime
  QTime curr_time = QTime::currentTime();
  if (curr_time.minute() == 0 && curr_time.second() == 0 &&
      !(_buffer_states & 0x4000)) {
    emit voicePlay(
        QUrl::fromLocalFile(QDir::currentPath() +
                            MediaInfoArray::chime_info[curr_time.hour()].file));
    emit voiceLabelSetText(MediaInfoArray::chime_info[curr_time.hour()].text);
  }
}

void FSM::onGLObjTaskFinished(const QString& task_name) {
  qDebug() << "[FSM] Task finished: " << task_name;
  if (task_name == "DRAStartMotion" && _fsm_status == START &&
      (_buffer_states & 0x7) == 0x7) {
    _fsm_status = IDLE;
    _buffer_states &= ~0x07;
  }
}
