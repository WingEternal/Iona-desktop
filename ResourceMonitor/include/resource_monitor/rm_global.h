#ifndef IONADESKTOP_RM_GLOBAL_H
#define IONADESKTOP_RM_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(RESOURCE_MONITOR_LIBRARY)
#  define RESOURCE_MONITOR_EXPORT Q_DECL_EXPORT
#else
#  define RESOURCE_MONITOR_EXPORT Q_DECL_IMPORT
#endif

#include "core/plugin_interface.h"
#include "resource_monitor/monitor_widget.h"
#include "resource_monitor/monitor_realization.h"

using namespace IonaDesktop::ResourceMonitor;

class RESOURCE_MONITOR_EXPORT  ResourceMonitor : public PluginBase
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.dist.interface")
    Q_INTERFACES(PluginBase)
public:
    ResourceMonitor();
    ~ResourceMonitor() override;
    __int64 getID() const override
    { return 0x01; }
    int onInit(QObject* parent) override;
private:
    MonitorWidget* monitor_widget_ptr;

    QThread *worker_thread_ptr;
    GeneralWorker *general_worker_ptr;
};

#endif // IONADESKTOP_RM_GLOBAL_H
