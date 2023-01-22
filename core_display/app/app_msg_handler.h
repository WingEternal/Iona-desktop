#ifndef IONADESKTOP_CORE_DISPLAY_APP_MSG_HANDLER_H
#define IONADESKTOP_CORE_DISPLAY_APP_MSG_HANDLER_H

#include <QMap>
#include <QList>
#include <QObject>

namespace IonaDesktop {
namespace CoreDisplay {

    typedef QString SID_t;

    class AppMsgHandler : public QObject
    {
        Q_OBJECT
    public:
        static AppMsgHandler& getInstance()
        {
            static AppMsgHandler msg_handler;
            return msg_handler;
        }
        void regSignal(const SID_t& sid, QObject* obj, const char* method);
        void bindSlot(const SID_t& sid, QObject* obj, const char* method, const Qt::ConnectionType c_type = Qt::AutoConnection);
        void installHandler();

        void listenEvent(const SID_t& sid, QObject* obj);
        void unlistenEvent(const SID_t& sid, QObject* obj);
        bool postEvent(const SID_t& sid, QEvent* ev, Qt::EventPriority = Qt::NormalEventPriority);
        bool sendEvent(const SID_t& sid, QEvent* ev);

    private:
        explicit AppMsgHandler();
        AppMsgHandler(const AppMsgHandler &) = delete;
        AppMsgHandler(const AppMsgHandler &&) = delete;
        ~AppMsgHandler();

        struct TransInfo
        {
            SID_t signal_id;
            QObject* object;
            const char* method;
            Qt::ConnectionType connection_type = Qt::AutoConnection;
        };

        typedef QMap<QObject*, TransInfo*> InfoMap;
        QMap<SID_t, InfoMap> m_sig_connect;
        QMap<SID_t, InfoMap> m_slot_connect;

        QMap<SID_t, QObject*> m_ev_listener;

        // Unused further feature
        // QMap<QObject*, InfoMap> m_sig_maintain;
        // QMap<QObject*, InfoMap> m_slot_maintain;
        // The signals and slots added after installHandler() is called need to be connected independently, and cannot be traversed in the map to prevent repeated connection
        // bool flag_active;
    };

}
}
#endif // IONADESKTOP_CORE_DISPLAY_APP_MSG_HANDLER_H
