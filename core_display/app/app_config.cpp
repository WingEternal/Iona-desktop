#include "app/app_config.h"
#include "app/app_msg_handler.h"
#include <QFileInfo>
#include <QTextCodec>
#include <QThread>
#include <QRect>
#include <QPoint>

using namespace IonaDesktop::CoreDisplay;

// AppConfigWorker* AppConfig::instance = nullptr;
// QThread* AppConfig::thrd = nullptr;

AppConfigWorker& AppConfig::getInstance()
{
    static AppConfig config_singleton;
    return *config_singleton.instance;
}

AppConfig::AppConfig()
    : instance(new AppConfigWorker),
      thrd(new QThread)
{
    instance->moveToThread(thrd);
    connect(thrd, &QThread::finished, instance, &QObject::deleteLater);
    thrd->start();
}

AppConfig::~AppConfig()
{
    thrd->quit();
    thrd->wait();
}

AppConfigWorker::AppConfigWorker()
    : QObject(nullptr)
{
    // Read ini
    settings = new QSettings("./config.ini", QSettings::IniFormat, this);
//    settings->setIniCodec(QTextCodec::codecForName("UTF-8"));

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
    {"/animate/ring_spin_rpm", 1.5},
    {"/animate/ring_tb_roll_spd", 25},
    {"/animate/ring_m_animation_switch_by_frame", 3},
    {"/move_ctrl/pos", QPoint(450, 350)},
    {"/quick_cmds/pos", QPoint(450, 300)}
};
