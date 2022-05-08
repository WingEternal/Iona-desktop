#include <QApplication>
#include <QTextCodec>
#include "core/base_widget.h"
#include "core/native_event_filter.h"
using namespace IonaDesktop::Core;

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(resources);
    QApplication a(argc, argv);

#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
#if _MSC_VER
    QTextCodec *codec = QTextCodec::codecForName("GBK");
#else
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
#endif
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#else
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
#endif
    NativeEventFilter filter;
    a.installNativeEventFilter(&filter);
    SingletonWarpper::getInstance()->show();
    return a.exec();
}
