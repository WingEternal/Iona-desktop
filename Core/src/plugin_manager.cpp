#include "plugin_core/plugin_manager.h"
#include <QDebug>

using namespace PluginCore;

PluginInstance::PluginInstance(QObject* parent, QString path)
{qDebug()<<"test";
    loader_ptr = QSharedPointer<QPluginLoader>(new QPluginLoader(path));
    QObject* pluginObjPtr = loader_ptr->instance();
    if(pluginObjPtr)
    {
        plugin_handler = qobject_cast<PluginBase*>(pluginObjPtr);
        if(plugin_handler){
            this->plugin_handler->onInit(parent);
            qDebug() << "Load Plugin: " << this->plugin_handler->getName();
        }
    }
}

PluginInstance::~PluginInstance()
{
    delete plugin_handler;
    loader_ptr->unload();
    qDebug() << "Unload Plugin: " << this->plugin_handler->getName();
}

PluginManager::PluginManager()
{
    QDir plugins_dir("./lib");
    if(plugins_dir.entryList(QDir::Files).isEmpty())
        qDebug() << "No Plugin.";
    foreach(QString filename, plugins_dir.entryList(QDir::Files))
    {
        auto plugin = QSharedPointer<PluginInstance>
                (new PluginInstance(qobject_cast<QObject*>(this),
                plugins_dir.absoluteFilePath(filename)));
        V_plugins.insert(plugin->getHandler()->getName(), plugin);
    }
}

