#include "core/core_global.h"
#include "core/plugin_instance.h"

using namespace IonaDesktop::Core;

PluginInstance::PluginInstance(QWidget* parent, QString path)
{
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
