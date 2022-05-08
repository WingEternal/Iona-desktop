#ifndef IONADESKTOP_CORE_NATIVE_EVENT_FILTER_H
#define IONADESKTOP_CORE_NATIVE_EVENT_FILTER_H
#include <QAbstractNativeEventFilter>
namespace IonaDesktop {
namespace Core {
    class NativeEventFilter : public QAbstractNativeEventFilter
    {
    public:
        NativeEventFilter();
        virtual bool nativeEventFilter
            (const QByteArray &eventType, void *message, long *longMsg) override;
    private:
        bool lmouse_pressed;
    };
}
}
#endif // IONADESKTOP_CORE_NATIVE_EVENT_FILTER_H
