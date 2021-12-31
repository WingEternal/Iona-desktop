#include "resource_monitor/rm_global.h"

ResourceMonitor::ResourceMonitor()
{
    name = "Resource_Monitor";
    version = "0.0.1";
    description = "Display a resouce monitor widget on screen.";
}
ResourceMonitor::~ResourceMonitor()
{
    worker_thread_ptr->quit();
    worker_thread_ptr->wait();
}
int ResourceMonitor::onInit(QObject* parent)
{
    monitor_widget_ptr = new MonitorWidget(dynamic_cast<QWidget*>(parent));
    monitor_widget_ptr->show();

    general_worker_ptr = new GeneralWorker();
    worker_thread_ptr = new QThread();
    general_worker_ptr->moveToThread(worker_thread_ptr);
    connect(worker_thread_ptr, SIGNAL(finished()), general_worker_ptr, SLOT(deleteLater()));

    connect(monitor_widget_ptr, SIGNAL(requestCpuUsage()), general_worker_ptr, SLOT(Slot_StatisticsCpuUsage()));
    connect(monitor_widget_ptr, SIGNAL(requestMemUsage()), general_worker_ptr, SLOT(Slot_StatisticsMemUsage()));

    connect(general_worker_ptr, SIGNAL(CpuUsageResultReady(const int)), monitor_widget_ptr, SLOT(Slot_CpuUsageResultReady(const int)));
    connect(general_worker_ptr, SIGNAL(MemUsageResultReady(const int)), monitor_widget_ptr, SLOT(Slot_MemUsageResultReady(const int)));
    worker_thread_ptr->start();
    return 0;
}
