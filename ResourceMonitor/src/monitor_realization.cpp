#include "resource_monitor/monitor_realization.h"
#include <QDebug>

using namespace IonaDesktop::ResourceMonitor;

GeneralWorker::GeneralWorker(QObject *parent) :
    QObject (parent) {}

void GeneralWorker::Slot_StatisticsCpuUsage()
{
#ifdef Q_OS_WIN
    static FILETIME prev_idle_time;
    static FILETIME prev_kernel_time;
    static FILETIME prev_user_time;

    FILETIME idle_time;
    FILETIME kernel_time;
    FILETIME user_time;

    GetSystemTimes(&idle_time, &kernel_time, &user_time);
    __int64 prev_time, curr_time;
    prev_time = (prev_idle_time.dwHighDateTime << 31) | prev_idle_time.dwLowDateTime;
    curr_time = (idle_time.dwHighDateTime << 31) | idle_time.dwLowDateTime;
    __int64 idle_diff = curr_time - prev_time;
    prev_time = (prev_kernel_time.dwHighDateTime << 31) | prev_kernel_time.dwLowDateTime;
    curr_time = (kernel_time.dwHighDateTime << 31) | kernel_time.dwLowDateTime;
    __int64 kernel_diff = curr_time - prev_time;
    prev_time = (prev_user_time.dwHighDateTime << 31) | prev_user_time.dwLowDateTime;
    curr_time = (user_time.dwHighDateTime << 31) | user_time.dwLowDateTime;
    __int64 user_diff = curr_time - prev_time;

    int cpu_usage = static_cast<int>((kernel_diff + user_diff - idle_diff) * 100 / (kernel_diff + user_diff));
    prev_idle_time = idle_time;
    prev_kernel_time = kernel_time;
    prev_user_time= user_time;
#endif

#ifdef Q_OS_LINUX
    static __int64 prev_cpu_total = 0;
    static __int64 prev_cpu_idle = 0;
    /* in /proc/stat: user | nice | system | idle | iowait | irq | softirq */
    QProcess process;
    process.start("cat /proc/stat");
    process.waitForFinished();
    QString stat_str = process.readLine();
    stat_str.replace("\n"," ");
    stat_str.replace(QRegExp("( ){1,}")," ");
    auto cpu_time = stat_str.split(" ");
    if(cpu_time.size() >= 7)
    {
        __int64 cpu_total = 0;
        for(int i = 1; i < cpu_time.size(); i++)
            cpu_total += static_cast<__int64>(cpu_time[i].toDouble());
        __int64 cpu_idle = static_cast<__int64>(cpu_time[3].toDouble());
        if(cpu_total - prev_cpu_total > 0)
        {
            int cpu_usage = static_cast<int>(1 - (cpu_idle - prev_cpu_idle) / (cpu_total - prev_cpu_total));
            prev_cpu_total = cpu_total;
            prev_cpu_idle = cpu_idle;
        }
    }

#endif
    if(cpu_usage < 0)
        cpu_usage = -1;
    if(cpu_usage > 100)
        cpu_usage = 100;
    emit CpuUsageResultReady(cpu_usage);
}

void GeneralWorker::Slot_StatisticsMemUsage()
{
#ifdef Q_OS_WIN
    MEMORYSTATUSEX mem_stat;
    mem_stat.dwLength = sizeof (mem_stat);
    if(!GlobalMemoryStatusEx(&mem_stat))
        emit MemUsageResultReady(-1);
    double mem_free = mem_stat.ullAvailPhys / 1024. / 1024.;
    double mem_total = mem_stat.ullTotalPhys / 1024. / 1024.;
    double mem_used = mem_total - mem_free;
    emit MemUsageResultReady(static_cast<int>(mem_used * 100 / mem_total));
#endif
}
