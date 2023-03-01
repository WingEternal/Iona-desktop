#include "app/app_native_ev_filter.h"
#include "app/app_msg_handler.h"
#include <QByteArray>
#include <QDebug>

using namespace IonaDesktop::CoreDisplay;

#ifdef Q_OS_WIN
#include "Windows.h"
#include "windowsx.h"
const UINT AppNativeEvFilter::WM_IONAGL_LOOKAT = RegisterWindowMessage(L"WM_IONAGL_LOOKAT");
#endif

AppNativeEvFilter::AppNativeEvFilter()
    : QAbstractNativeEventFilter()
{
    AppMsgHandler::getInstance().regSignal("/mouse", this, SIGNAL(sendGlMouseEvent(const QEvent::Type, const Qt::MouseButton, const QPoint)));
}

bool AppNativeEvFilter::nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result)
{
#ifdef Q_OS_WIN
    if (eventType == "windows_generic_MSG" || eventType == "windows_dispatcher_MSG")
    {
        MSG* pMsg = reinterpret_cast<MSG*>(message);
        if (pMsg->message == WM_IONAGL_LOOKAT)
        {
            switch(pMsg->wParam)
            {
            case WM_LBUTTONDOWN:
                emit sendGlMouseEvent
                    (QEvent::MouseButtonPress, Qt::LeftButton,
                    QPoint(GET_X_LPARAM(pMsg->lParam), GET_Y_LPARAM(pMsg->lParam)));
                break;
            case WM_MOUSEMOVE:
                emit sendGlMouseEvent
                    (QEvent::MouseMove, Qt::LeftButton,
                    QPoint(GET_X_LPARAM(pMsg->lParam), GET_Y_LPARAM(pMsg->lParam)));
                break;
            case WM_LBUTTONUP:
                emit sendGlMouseEvent
                    (QEvent::MouseButtonRelease, Qt::LeftButton,
                    QPoint(GET_X_LPARAM(pMsg->lParam), GET_Y_LPARAM(pMsg->lParam)));
                break;
            }
            return true;
        }
        else return false;
    }
#endif
    return false;
}
