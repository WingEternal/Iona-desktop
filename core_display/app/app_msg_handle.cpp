#include "app_msg_handle.h"
#include <QCoreApplication>
#include <QDebug>

using namespace IonaDesktop::CoreDisplay;

AppMsgHandler::AppMsgHandler()
    : QObject(nullptr)
{

}

AppMsgHandler::~AppMsgHandler()
{
    for(auto im_sig : m_sig_connect)
        for(auto ti_sig : im_sig)
            delete ti_sig;
    for(auto im_slot : m_slot_connect)
        for(auto ti_slot : im_slot)
            delete ti_slot;
}

void AppMsgHandler::regSignal(const SID_t& sid, QObject* obj, const char* method)
{
    if(m_sig_connect.find(sid) == m_sig_connect.end())
    {
        InfoMap new_info_map;
        m_sig_connect.insert(sid, new_info_map);
    }
    if(m_slot_connect[sid].find(obj) == m_slot_connect[sid].end())
    {
        TransInfo* ti = new TransInfo();
        m_sig_connect[sid].insert(obj, ti);
    }
    m_sig_connect[sid][obj]->signal_id = sid;
    m_sig_connect[sid][obj]->object = obj;
    m_sig_connect[sid][obj]->method = method;
}

void AppMsgHandler::bindSlot(const SID_t &sid, QObject *obj, const char *method, const Qt::ConnectionType c_type)
{
    if(m_slot_connect.find(sid) == m_slot_connect.end())
    {
        InfoMap new_info_map;
        m_slot_connect.insert(sid, new_info_map);
    }
    if(m_slot_connect[sid].find(obj) == m_slot_connect[sid].end())
    {
        TransInfo* ti = new TransInfo();
        m_slot_connect[sid].insert(obj, ti);
    }
    m_slot_connect[sid][obj]->signal_id = sid;
    m_slot_connect[sid][obj]->object = obj;
    m_slot_connect[sid][obj]->method = method;
    m_slot_connect[sid][obj]->connection_type = c_type;
}

void AppMsgHandler::installHandle()
{
    for(auto im_sig : m_sig_connect)
        for(auto ti_sig : im_sig)
            for(auto ti_slot : m_slot_connect[ti_sig->signal_id])
            {
                connect(ti_sig->object, ti_sig->method, ti_slot->object,
                        ti_slot->method, ti_slot->connection_type);
                qDebug() << "SIGNAL " << ti_sig->method << " SLOT" << ti_slot->method;
            }
}

void AppMsgHandler::listenEvent(const SID_t &sid, QObject* obj)
{
    if(m_ev_listener.find(sid) == m_ev_listener.end())
        m_ev_listener.insert(sid, obj);
    else m_ev_listener[sid] = obj;
}

void AppMsgHandler::unlistenEvent(const SID_t &sid, QObject *obj)
{
    auto ev_iter = m_ev_listener.find(sid);
    if(ev_iter != m_ev_listener.end() && ev_iter.value() == obj)
        m_ev_listener[sid] = nullptr;
}

bool AppMsgHandler::postEvent(const SID_t &sid, QEvent *ev, Qt::EventPriority pri)
{
    auto ev_iter = m_ev_listener.find(sid);
    if(ev_iter == m_ev_listener.end() && ev_iter.value() != nullptr)
        return false;
    QCoreApplication::postEvent(ev_iter.value(), ev, pri);
    return true;
}

bool AppMsgHandler::sendEvent(const SID_t &sid, QEvent *ev)
{
    auto ti_iter = m_ev_listener.find(sid);
    if(ti_iter == m_ev_listener.end())
        return false;
    QCoreApplication::sendEvent(ti_iter.value(), ev);
    return true;
}
