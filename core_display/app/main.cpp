#include <QApplication>
#include <QTextCodec>
#include <QSharedMemory>

#include "app/app_msg_handler.h"
#include "app/app_param.h"
#include "app/app_native_ev_filter.h"
#include "app/app_content.h"
#include "app/app_thread_helper.hpp"
#include "voice/wav_handler.h"
#include "fsm/fsm.h"

using namespace IonaDesktop::CoreDisplay;

int main(int argc, char** argv)
{
    Q_INIT_RESOURCE(resources);
    QApplication app(argc, argv);

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);

    QSharedMemory singleton_shared("Iona-desktop-core-display");
    if(singleton_shared.attach())
    {
        qDebug() << "[APP] CoreDisplay(Singleton) is already running. Exit.";
        return 0;
    }
    singleton_shared.create(1);

    // Load app singletons
    AppMsgHandler::getInstance();
    AppParam::getInstance();

    // Init native event filter for receiving mouse / keyboard message
    AppNativeEvFilter* native_evf = new AppNativeEvFilter();
    app.installNativeEventFilter(native_evf);

    // Init GUI
    AppContent* display = new AppContent();

    // Init utils
    auto* wav_ctrl = new ThrdCtrl<WavHandler>();
    auto* fsm_ctrl = new ThrdCtrl<FSM>();
    
    // Start application
    fsm_ctrl->start();
    display->show();
    return app.exec();
}
