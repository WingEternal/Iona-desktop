#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

#include <QDir>
#include <QFile>
#include <QMap>
#include <QPluginLoader>
#include <QDebug>
#include "plugin_interface.h"

namespace PluginCore {
    // Storage pointer of plugin's loader && plugin instance
    class PluginInstance
    {
    public:
        PluginInstance(QObject* parent, QString path);
        ~PluginInstance();

        inline PluginBase* getHandler()
        { return plugin_handler; }
    private:
        QSharedPointer<QPluginLoader> loader_ptr;
        PluginBase* plugin_handler;
    };

    class PluginManager : public QObject
    {
        Q_OBJECT
    public:
        PluginManager();
        ~PluginManager(){}
    private:
        QMap<QString, QSharedPointer<PluginInstance>> V_plugins;
    };
}

#endif // PLUGIN_MANAGER_H
