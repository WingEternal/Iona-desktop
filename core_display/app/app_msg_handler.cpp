#include "app/app_msg_handler.h"
#include <QCoreApplication>
#include <QDebug>

using namespace IonaDesktop::CoreDisplay;

AppMsgHandler::AppMsgHandler()
    : QObject(nullptr)
{

}

AppMsgHandler::~AppMsgHandler()
{

}

void AppMsgHandler::regSignal(const SID_t& sid, QObject* obj, const char* method)
{
    std::lock_guard guard(_hc_lock);
    if(_hash_connect.find(sid) == _hash_connect.end())
    {
        SigTuple new_tuple;
        _hash_connect.insert(sid, new_tuple);
    }
    auto& curr_tuple = _hash_connect[sid];
    for(auto sig_info = curr_tuple.vsig_infos.constBegin(); sig_info != curr_tuple.vsig_infos.constEnd(); sig_info++)
        if((*sig_info)->object == obj && (*sig_info)->method == method)
        {
            qDebug() << "[MsgHandler] Signal " << method << " has already registed";
            return;
        }
    TransInfo* curr_ti = new TransInfo();
    curr_ti->object = obj;
    curr_ti->method = method;
    curr_tuple.vsig_infos.insert(curr_ti);

    // look for slots
    for(auto slot_info = curr_tuple.vslot_infos.begin(); slot_info != curr_tuple.vslot_infos.end(); slot_info++)
        if(!curr_ti->establised.contains(*slot_info))
        {
            if(connect(curr_ti->object, curr_ti->method, (*slot_info)->object,
                    (*slot_info)->method, (*slot_info)->connection_type))
            {
                curr_ti->establised.insert(*slot_info);
                (*slot_info)->establised.insert(curr_ti);
                qDebug() << "[MsgHandler] Established connection:"
                         << " Signal " << curr_ti->method
                         << " Slot " << (*slot_info)->method;
            }
            else
                qDebug() << "[MsgHandler] Invalid connection:"
                         << " Signal " << curr_ti->method
                         << " Slot " << (*slot_info)->method;
        }
}

void AppMsgHandler::bindSlot(const SID_t &sid, QObject *obj, const char *method, const Qt::ConnectionType c_type)
{
    std::lock_guard guard(_hc_lock);
    if(_hash_connect.find(sid) == _hash_connect.end())
    {
        SigTuple new_tuple;
        _hash_connect.insert(sid, new_tuple);
    }
    auto& curr_tuple = _hash_connect[sid];
    for(auto slot_info = curr_tuple.vslot_infos.constBegin(); slot_info != curr_tuple.vslot_infos.constEnd(); slot_info++)
        if((*slot_info)->object == obj && (*slot_info)->method == method)
        {
            qDebug() << "[MsgHandler] Slot " << method << " has already registed";
            return;
        }
    TransInfo* cur_ti = new TransInfo();
    cur_ti->object = obj;
    cur_ti->method = method;
    cur_ti->connection_type = c_type;
    curr_tuple.vslot_infos.insert(cur_ti);

    // look for sigs
    for(auto sig_info = curr_tuple.vsig_infos.begin(); sig_info != curr_tuple.vsig_infos.end(); sig_info++)
        if(!cur_ti->establised.contains(*sig_info))
        {
            if(connect((*sig_info)->object, (*sig_info)->method, cur_ti->object,
                    cur_ti->method, cur_ti->connection_type))
            {
                // Connection valid
                (*sig_info)->establised.insert(cur_ti);
                cur_ti->establised.insert(*sig_info);
                qDebug() << "[MsgHandler] Established connection:"
                         << " Signal " << (*sig_info)->method
                         << " Slot " << cur_ti->method;
            }
            else
                qDebug() << "[MsgHandler] Invalid connection:"
                         << " Signal " << (*sig_info)->method
                         << " Slot " << cur_ti->method;
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
