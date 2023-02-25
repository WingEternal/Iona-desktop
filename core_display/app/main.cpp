#include <QApplication>
#include <QTextCodec>

#include "app/app_msg_handler.h"
#include "app/app_config.h"
#include "app/app_native_ev_filter.h"
#include "app/app_widget_container.h"
#include "app/app_thread_helper.hpp"
#include "voice/wav_handler.h"
#include "asm/asm.h"

using namespace IonaDesktop::CoreDisplay;

int main(int argc, char** argv)
{
    Q_INIT_RESOURCE(resources);
    QApplication app(argc, argv);

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);

    // Load app singletons
    AppMsgHandler::getInstance();
    AppConfig::getInstance();

    // Init native event filter for receiving mouse / keyboard message
    AppNativeEvFilter* native_evf = new AppNativeEvFilter();
    app.installNativeEventFilter(native_evf);

    // Init GUI
    AppWidgetContainer* display = new AppWidgetContainer();

    // Init utils
    auto* wav_ctrl = new ThrdCtrl<WavHandler>();
    auto* asm_ctrl = new ThrdCtrl<ASM>();
    
    // Start application
    asm_ctrl->start();
    display->show();
    return app.exec();
}
