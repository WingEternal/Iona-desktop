#ifndef  IONADESKTOP_RM_MONITOR_REALIZATION_H
#define IONADESKTOP_RM_MONITOR_REALIZATION_H

#include <QtGlobal>
#include <QObject>
#include <QThread>
#include <QProcess>

#ifdef Q_OS_WIN
#include "windows.h"
#endif

namespace IonaDesktop {
namespace ResourceMonitor {
    class GeneralWorker : public QObject
    {
        Q_OBJECT
    public:
        explicit GeneralWorker(QObject *parent = nullptr);
        ~GeneralWorker(){}
    protected:
    signals:
        void CpuUsageResultReady(const int cpu_usage);
        void MemUsageResultReady(const int mem_usage);
    public slots:
        void Slot_StatisticsCpuUsage();
        void Slot_StatisticsMemUsage();
    };
}
}

#endif // IONADESKTOP_RM_MONITOR_REALIZATION_H
