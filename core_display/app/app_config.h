#ifndef IONADESKTOP_CORE_DISPLAY_APP_CONFIG_H
#define IONADESKTOP_CORE_DISPLAY_APP_CONFIG_H

#include <QObject>
#include <QSettings>
#include <QHash>
#include <QDebug>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

namespace IonaDesktop {
namespace CoreDisplay {
    class AppConfigWorker;

    class AppConfig : public QObject
    {
        Q_OBJECT
    public:
        static AppConfigWorker& getInstance();
    private:
        static AppConfigWorker* instance;
        static QThread* thrd;
    };

    class AppConfigWorker : public QObject
    {
        Q_OBJECT
        friend class AppConfig;
    public:
        template<typename T>
        bool getParam(const QString& key, T& value) const
        {
            bool flag_cfg_has_key = settings->contains(key);
            bool flag_default_has_key = default_value.contains(key);
            if(!flag_cfg_has_key && !flag_default_has_key)
                return false;
            if(!flag_cfg_has_key)
                emit addDefaultParam(key, default_value.value(key));
            auto val = flag_cfg_has_key? settings->value(key) : default_value.value(key);
            if(!val.canConvert<T>())
            {
                qDebug() << "[AppConfig]: invalid convert in " << key << ", abort.";
                return false;
            }
            value = val.value<T>();
            return true;
        }

    private:
        explicit AppConfigWorker();
        ~AppConfigWorker();

        QSettings* settings;
        static const QHash<QString, QVariant> default_value;

    signals:
        void addDefaultParam(const QString key, const QVariant value) const;

    public slots:
        void setParam(const QString& key, const QVariant& value)
        {
            settings->setValue(key, value);
//            qDebug() << "set value: " << key << " v: " << value;
//            settings->sync();
        }
    };

}
}

#endif // IONADESKTOP_CORE_DISPLAY_APP_CONFIG_H
