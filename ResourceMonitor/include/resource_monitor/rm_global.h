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
#include "resource_monitor/rm_widget.h"
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
    QThread worker_thread;
    GeneralWorker *general_worker_ptr;

    QTimer *refresh_timer_ptr;

    void SetupWidgets(QWidget* parent);
    const int hexagon_size;
    RmWidget *cpu_usage_widget_ptr;
    RmWidget *mem_usage_widget_ptr;
    RmWidget *time_widget_ptr;
    RmWidget *stb_2_widget_ptr;
    RmWidget *stb_3_widget_ptr;
    void UpdateTime();
signals:
    void requestCpuUsage();
    void requestMemUsage();
private slots:
    void Slot_Refresh_Timeout();
    void Slot_Update_CPU(const double new_rate);
    void Slot_Update_MEM(const double new_rate);
};

#endif // IONADESKTOP_RM_GLOBAL_H
