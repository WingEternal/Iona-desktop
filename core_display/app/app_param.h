#ifndef IONADESKTOP_CORE_DISPLAY_APP_PARAM_H
#define IONADESKTOP_CORE_DISPLAY_APP_PARAM_H

#include <QObject>
#include <QThread>
#include <QReadWriteLock>
#include <QSettings>
#include <QHash>
#include <QDebug>
#include <QSharedPointer>

namespace IonaDesktop {
namespace CoreDisplay {
    class AppParamWorker;
    class AppParam : public QObject
    {
        Q_OBJECT
    public:
        static AppParamWorker& getInstance();
    private:
        explicit AppParam();
        ~AppParam();
        AppParamWorker* instance;
        QThread* thrd;
    };

    class AppParamWorker : public QObject
    {
        Q_OBJECT
        friend class AppParam;
    public:
        template<typename T>
        bool getParam(const QString& key, T& value)
        {
            bool flag_cfg_has_key = settings->contains(key);
            bool flag_dyn_has_key = dyn_value.contains(key);
            if(!flag_cfg_has_key && !flag_dyn_has_key)
                return false;
            QVariant val;
            if(flag_cfg_has_key)
                val = settings->value(key);
            else {
                QReadLocker rlocker(&dyn_value[key]->lock);
                val = dyn_value[key]->value;
            }
            if(!val.canConvert<T>())
            {
                qDebug() << "[AppParam]: invalid convert in " << key << ", abort.";
                return false;
            }
            value = val.value<T>();
            return true;
        }

    private:
        explicit AppParamWorker();
        ~AppParamWorker();

        QSettings* settings;
        static const QHash<QString, QVariant> default_value;
        struct ParamValue {
            QReadWriteLock lock;
            QVariant value;
        };
        QHash<QString, QSharedPointer<ParamValue>> dyn_value;

    public slots:
        void setParam(const QString& key, const QVariant& value);
    };
}
}

#endif // IONADESKTOP_CORE_DISPLAY_APP_PARAM_H
