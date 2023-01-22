#ifndef IONADESKTOP_CORE_DISPLAY_APP_NATIVE_EV_FILTER_H
#define IONADESKTOP_CORE_DISPLAY_APP_NATIVE_EV_FILTER_H

#include <QAbstractNativeEventFilter>
#include <QObject>
#include <QEvent>

#ifdef Q_OS_WIN
#include "hook.h"
#endif

namespace IonaDesktop {
namespace CoreDisplay {
    class AppNativeEvFilter : public QObject,  public QAbstractNativeEventFilter
    {
        Q_OBJECT
    public:
        AppNativeEvFilter();

        virtual bool nativeEventFilter
            (const QByteArray &eventType, void *message, long *longMsg) override;

    private:
#ifdef Q_OS_WIN
        static const UINT WM_IONAGL_LOOKAT;
#endif

    signals:
        void sendGlMouseEvent
            (const QEvent::Type type, const Qt::MouseButton b, const QPoint pt);
    };
}
}

#endif // IONADESKTOP_CORE_DISPLAY_APP_NATIVE_EV_FILTER_H
