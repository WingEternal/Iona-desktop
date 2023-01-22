#include "app_config.h"
#include "app_msg_handler.h"
#include <QFileInfo>
#include <QTextCodec>
#include <QThread>

using namespace IonaDesktop::CoreDisplay;

AppConfigWorker* AppConfig::instance = nullptr;
QThread* AppConfig::thrd = nullptr;

AppConfigWorker& AppConfig::getInstance()
{
    if(instance == nullptr)
    {
        instance = new AppConfigWorker();
        thrd = new QThread;
        instance->moveToThread(thrd);
        thrd->start();
    }
    return *instance;
}

AppConfigWorker::AppConfigWorker()
    : QObject(nullptr)
{
    // Read ini
    settings = new QSettings("./config.ini", QSettings::IniFormat, this);
    settings->setIniCodec(QTextCodec::codecForName("UTF-8"));

    // Bind slot
    connect(this, SIGNAL(addDefaultParam(const QString, const QVariant)),
        this, SLOT(setParam(const QString, const QVariant)), Qt::QueuedConnection);
    AppMsgHandler::getInstance().bindSlot
        ("/app/set_param", this, SLOT(setParam(const QString&, const QVariant&)), Qt::QueuedConnection);
//    AppMsgHandler::getInstance().bindSlot
//        ("/app/setting_sync", this, SLOT(sync()));
}

AppConfigWorker::~AppConfigWorker()
{

}

const QHash<QString, QVariant> AppConfigWorker::default_value = {
    {"/window/size", QRect(1300, 560, 600, 500)},
    {"/animate/update_rate", 30.0},
    {"/animate/ring_spin_rpm", 2.5},
    {"/move_ctrl/size", QRect(450, 300, 50, 50)}
};
