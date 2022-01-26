#ifndef IONADESKTOP_CORE_PLUGIN_INSTANCE_H
#define IONADESKTOP_CORE_PLUGIN_INSTANCE_H

#include <QWidget>
#include "core/plugin_interface.h"

namespace IonaDesktop {
namespace Core{
    class PluginInstance
    {
    public:
        PluginInstance(QWidget* parent, QString path);
        ~PluginInstance();
        inline PluginBase* getHandler()
        { return plugin_handler; }
    private:
        QSharedPointer<QPluginLoader> loader_ptr;
        PluginBase* plugin_handler;
    };
}
}

#endif // IONADESKTOP_CORE_PLUGIN_INSTANCE_H
