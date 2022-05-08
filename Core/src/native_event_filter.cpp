#include "core/native_event_filter.h"
#include "core/base_widget.h"
#include "hook.h"
#include <QApplication>
#include <QMouseEvent>
using namespace IonaDesktop::Core;

NativeEventFilter::NativeEventFilter()
    : QAbstractNativeEventFilter (),
      lmouse_pressed(false)
{}

bool NativeEventFilter::nativeEventFilter(const QByteArray &eventType, void *message, long *longMsg)
{
    Q_UNUSED(longMsg);
    if (eventType == "windows_generic_MSG" || eventType == "windows_dispatcher_MSG")
    {
        MSG* pMsg = reinterpret_cast<MSG*>(message);
        if (pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONUP || pMsg->message == WM_MOUSEMOVE)
        {
            POINT pt = pMsg->pt;
            QMouseEvent* ev;
            switch (pMsg->message) {
            case WM_LBUTTONDOWN:
                ev = new QMouseEvent
                    (QEvent::MouseButtonPress, QPointF(pt.x, pt.y),
                    Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
                lmouse_pressed = true;
                QApplication::postEvent
                    (SingletonWarpper::getInstance()->getDisplayInstance(), ev);
                break;
            case WM_MOUSEMOVE:
                if(lmouse_pressed)
                {
                    ev = new QMouseEvent
                        (QEvent::MouseMove, QPointF(pt.x, pt.y),
                         Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
                    QApplication::sendEvent
                        (SingletonWarpper::getInstance()->getDisplayInstance(), ev);
                }
                break;
            case WM_LBUTTONUP:
                ev = new QMouseEvent
                    (QEvent::MouseButtonRelease, QPointF(pt.x, pt.y),
                     Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
                lmouse_pressed = false;
                QApplication::sendEvent
                    (SingletonWarpper::getInstance()->getDisplayInstance(), ev);
                break;
            }
        }
//            if (pMsg->message == WM_KEYDOWN)
//                qDebug() << pMsg->wParam;
    }
    return false;
}
