#include "app_param.h"
#include "app_msg_handler.h"
#include <QFileInfo>
#include <QRect>
#include <QPoint>

using namespace IonaDesktop::CoreDisplay;

// AppConfigWorker* AppConfig::instance = nullptr;
// QThread* AppConfig::thrd = nullptr;

AppParamWorker& AppParam::getInstance()
{
    static AppParam config_singleton;
    return *config_singleton.instance;
}

AppParam::AppParam()
    : instance(new AppParamWorker),
      thrd(new QThread)
{
    instance->moveToThread(thrd);
    connect(thrd, &QThread::finished, instance, &QObject::deleteLater);
    thrd->start();
}

AppParam::~AppParam()
{
    thrd->quit();
    thrd->wait();
}

AppParamWorker::AppParamWorker()
    : QObject(nullptr)
{
    // Read ini
    settings = new QSettings("./config.ini", QSettings::IniFormat, this);
//    settings->setIniCodec(QTextCodec::codecForName("UTF-8"));

    // If default params doesn't exist, add to qsettings
    for(auto it = default_value.cbegin(); it != default_value.cend(); it++)
        if(!settings->contains(it.key()))
            settings->setValue(it.key(), it.value());

    // Bind slot
    AppMsgHandler::getInstance().bindSlot
        ("/app/set_param", this, SLOT(setParam(const QString&, const QVariant&)), Qt::QueuedConnection);
}

AppParamWorker::~AppParamWorker()
{

}

const QHash<QString, QVariant> AppParamWorker::default_value = {
    {"S$window/pos", QPoint(1300, 560)},
    {"S$animate/update_rate", 30.0},
    {"S$animate/ring_max_rpm", 3.0},
    {"S$animate/ring_tb_roll_spd", 25},
    {"S$animate/ring_m_animation_switch_by_frame", 3}
};

void AppParamWorker::setParam(const QString& key, const QVariant& value)
{
    if(settings->contains(key))
        settings->setValue(key, value);
    else
    {
        if(!dyn_value.contains(key))
            dyn_value.insert(key, QSharedPointer<ParamValue>(new ParamValue()));
        QWriteLocker wlocker(&dyn_value[key]->lock);
        dyn_value[key]->value = value;
    }
}
