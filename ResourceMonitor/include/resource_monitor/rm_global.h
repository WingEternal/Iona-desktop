#ifndef IONADESKTOP_RM_GLOBAL_H
#define IONADESKTOP_RM_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(RESOURCE_MONITOR_LIBRARY)
#  define RESOURCE_MONITOR_EXPORT Q_DECL_EXPORT
#else
#  define RESOURCE_MONITOR_EXPORT Q_DECL_IMPORT
#endif

#include <QTimer>
#include "core/plugin_interface.h"
#include "resource_monitor/hexagon_widget_progress.h"
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
    int32_t getID() const override
    { return 0x01; }
    int onInit(QWidget* parent) override;
private:
//    MonitorWidget* monitor_widget_ptr;

    QThread worker_thread;
    GeneralWorker *general_worker_ptr;

    QTimer refresh_timer;

    void SetupWidgets(QWidget* parent);
    const int hexagon_size;
    const int spacing_size;
    HexagonProgressWidget *cpu_usage_widget_ptr;
    HexagonProgressWidget *mem_usage_widget_ptr;
    HexagonProgressWidget *stb_1_widget_ptr;
    HexagonProgressWidget *stb_2_widget_ptr;
    HexagonProgressWidget *stb_3_widget_ptr;
signals:
    void requestCpuUsage();
    void requestMemUsage();
private slots:
    void Slot_Refresh_Timeout();
};

#endif // IONADESKTOP_RM_GLOBAL_H
