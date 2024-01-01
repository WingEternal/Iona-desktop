#ifndef IONADESKTOP_CORE_DISPLAY_APP_MSG_HANDLE_H
#define IONADESKTOP_CORE_DISPLAY_APP_MSG_HANDLE_H

#include <QHash>
#include <QList>
#include <QObject>
#include <QSet>
#include <QVector>
#include <mutex>

namespace IonaDesktop {
namespace CoreDisplay {

typedef QString SID_t;

class AppMsgHandler : public QObject {
  Q_OBJECT
 public:
  static AppMsgHandler& getInstance() {
    static AppMsgHandler msg_handler;
    return msg_handler;
  }
  void regSignal(const SID_t& sid, QObject* obj, const char* method);
  void bindSlot(const SID_t& sid, QObject* obj, const char* method,
                const Qt::ConnectionType c_type = Qt::AutoConnection);

  void listenEvent(const SID_t& sid, QObject* obj);
  void unlistenEvent(const SID_t& sid, QObject* obj);
  bool postEvent(const SID_t& sid, QEvent* ev,
                 Qt::EventPriority = Qt::NormalEventPriority);
  bool sendEvent(const SID_t& sid, QEvent* ev);

 private:
  explicit AppMsgHandler();
  AppMsgHandler(const AppMsgHandler&) = delete;
  AppMsgHandler(const AppMsgHandler&&) = delete;
  ~AppMsgHandler();

  struct TransInfo {
    QObject* object;
    const char* method;
    Qt::ConnectionType connection_type = Qt::AutoConnection;
    QSet<TransInfo*> establised;
  };
  struct SigTuple {
    QSet<TransInfo*> vsig_infos;
    QSet<TransInfo*> vslot_infos;
  };

  std::mutex _hc_lock;
  QHash<SID_t, SigTuple> _hash_connect;

  QHash<SID_t, QObject*> m_ev_listener;
};

}  // namespace CoreDisplay
}  // namespace IonaDesktop
#endif  // IONADESKTOP_CORE_DISPLAY_APP_MSG_HANDLE_H
