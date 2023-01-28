#include <QApplication>
#include <QTextCodec>
#include "app_msg_handle.h"
#include "app_config.h"
#include "app_native_ev_filter.h"
#include "app_widget_container.h"

using namespace IonaDesktop::CoreDisplay;

int main(int argc, char** argv)
{
    Q_INIT_RESOURCE(resources);
    QApplication app(argc, argv);

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);

    AppMsgHandler::getInstance();
    AppConfig::getInstance();

    AppNativeEvFilter* native_evf = new AppNativeEvFilter();
    app.installNativeEventFilter(native_evf);

    AppWidgetContainer* display = new AppWidgetContainer();

    AppMsgHandler::getInstance().installHandle();
    display->show();
    return app.exec();
}
