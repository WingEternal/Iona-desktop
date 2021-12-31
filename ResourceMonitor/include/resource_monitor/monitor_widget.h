#ifndef IONADESKTOP_RM_MONITOR_WIDGET_H
#define IONADESKTOP_RM_MONITOR_WIDGET_H

#include <QWidget>
#include <QTimer>

#include "resource_monitor/hexagon_widget.h"

namespace Ui {
class MonitorWidget;
}
namespace IonaDesktop {
namespace ResourceMonitor {
    class MonitorWidget : public QWidget
    {
        Q_OBJECT

    public:
        explicit MonitorWidget(QWidget *parent = nullptr);
        ~MonitorWidget();

    private:
        Ui::MonitorWidget *ui;

        const int hexagon_size;
        const int spacing_size;
        HexagonWidget *cpu_usage_widget_ptr;
        HexagonWidget *mem_usage_widget_ptr;
        HexagonWidget *stb_1_widget_ptr;
        HexagonWidget *stb_2_widget_ptr;
        HexagonWidget *stb_3_widget_ptr;

        QTimer *refresh_timer_ptr;
    protected:
        void paintEvent(QPaintEvent *event);
    signals:
        // Signals to worker threads
        void requestCpuUsage();
        void requestMemUsage();
        // Signals to hexagon widgets
        void sendCpuUsage(const int cpu_usage);
        void sendMemUsage(const int mem_usage);
    public slots:
        void Slot_CpuUsageResultReady(const int cpu_usage);
        void Slot_MemUsageResultReady(const int mem_usage);
    private slots:
        void Slot_Refresh_Timeout();
    };
}
}


#endif // IONADESKTOP_RM_MONITOR_WIDGET_H
