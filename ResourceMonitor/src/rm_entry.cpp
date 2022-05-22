#include <QtMath>
#include <QDateTime>

#include "resource_monitor/rm_global.h"

ResourceMonitor::ResourceMonitor()
    : hexagon_size(83)
{
    name = "Resource_Monitor";
    version = "0.0.1";
    description = "Display a resouce monitor widget on screen.";
}
ResourceMonitor::~ResourceMonitor()
{
    worker_thread.quit();
    worker_thread.wait();
}
int ResourceMonitor::onInit(QWidget* parent)
{
    SetupWidgets(parent);

    general_worker_ptr = new GeneralWorker();
    general_worker_ptr->moveToThread(&worker_thread);
    // Begin SIG-SLOT
    connect(this, SIGNAL(requestCpuUsage()), general_worker_ptr, SLOT(Slot_StatisticsCpuUsage()));
    connect(this, SIGNAL(requestMemUsage()), general_worker_ptr, SLOT(Slot_StatisticsMemUsage()));
    // Return Result SIG-SLOT
    connect(general_worker_ptr, SIGNAL(CpuUsageResultReady(const double)), this, SLOT(Slot_Update_CPU(const double)));
    connect(general_worker_ptr, SIGNAL(MemUsageResultReady(const double)), this, SLOT(Slot_Update_MEM(const double)));
    // End SIG-SLOT
    connect(&worker_thread, SIGNAL(finished()), general_worker_ptr, SLOT(deleteLater()));
    worker_thread.start();

    // Timer SIG-SLOT
    refresh_timer_ptr = new QTimer(this);
    connect(refresh_timer_ptr, SIGNAL(timeout()), this, SLOT(Slot_Refresh_Timeout()));
    refresh_timer_ptr->start(1000);
    return 0;
}
void ResourceMonitor::SetupWidgets(QWidget* parent)
{
    const int widgets_origin_w = 125;
    const int widgets_origin_h = 270;

    cpu_usage_widget_ptr = new RmWidget(QRect(widgets_origin_w - hexagon_size / 2, widgets_origin_h - hexagon_size / 2, hexagon_size, hexagon_size), "CPU", parent);

    mem_usage_widget_ptr = new RmWidget(QRect(qRound(widgets_origin_w - 1.25 * hexagon_size), qRound(widgets_origin_h - (sqrt(3) + 2) / 4 * hexagon_size), hexagon_size, hexagon_size), "MEM", parent);

    time_widget_ptr = new RmWidget(QRect(widgets_origin_w - hexagon_size / 2, qRound(widgets_origin_h - (sqrt(3) + 1) / 2 * hexagon_size), hexagon_size, hexagon_size), "DATE", parent);

    stb_2_widget_ptr = new RmWidget(QRect(qRound(widgets_origin_w + 0.25 * hexagon_size), qRound(widgets_origin_h - (sqrt(3) + 2) / 4 * hexagon_size), hexagon_size, hexagon_size), "NULL", parent);

    stb_3_widget_ptr = new RmWidget(QRect(qRound(widgets_origin_w + 0.25 * hexagon_size), qRound(widgets_origin_h - (3 * sqrt(3) + 2) / 4 * hexagon_size), hexagon_size, hexagon_size), "NULL", parent);
}
void ResourceMonitor::Slot_Refresh_Timeout()
{
    emit requestCpuUsage();
    emit requestMemUsage();
    UpdateTime();
}

void ResourceMonitor::Slot_Update_CPU(const double new_rate)
{
    cpu_usage_widget_ptr->setRate(QString("%1").arg(QString::number(new_rate, 'g', 3)));
    cpu_usage_widget_ptr->update();
}

void ResourceMonitor::Slot_Update_MEM(const double new_rate)
{
    mem_usage_widget_ptr->setRate(QString("%1").arg(QString::number(new_rate, 'g', 3)));
    mem_usage_widget_ptr->update();
}

void ResourceMonitor::UpdateTime()
{
    auto date_time = QDateTime::currentDateTime();
    time_widget_ptr->setRate(date_time.toString("HHmm"));
    time_widget_ptr->setName(date_time.toString("yy|MM|dd"));
    time_widget_ptr->update();
}
