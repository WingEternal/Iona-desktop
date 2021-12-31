#include "resource_monitor/monitor_widget.h"
#include "ui_monitor_widget.h"

#include <QtMath>
#include <QDebug>
#include <QPainter>
#include <QPaintEvent>

using namespace IonaDesktop::ResourceMonitor;

MonitorWidget::MonitorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MonitorWidget),
    hexagon_size(80), spacing_size(8)
{
    ui->setupUi(this);
//    setWindowFlag(Qt::Window);
//    setWindowFlag(Qt::FramelessWindowHint);
//    setAttribute(Qt::WA_TranslucentBackground);
//    setWindowFlag(Qt::Tool);
    setGeometry(0, 120, 250, 300);

    cpu_usage_widget_ptr = new HexagonWidget("CPU", this);
    // set to center point
    cpu_usage_widget_ptr->setGeometry((width() - hexagon_size) / 2, (height() - hexagon_size) / 2, hexagon_size, hexagon_size);
    cpu_usage_widget_ptr->show();
    connect(this, SIGNAL(sendCpuUsage(const int)), cpu_usage_widget_ptr, SLOT(Slot_UpdateRate(const int)));

    mem_usage_widget_ptr = new HexagonWidget("MEM", this);
    mem_usage_widget_ptr->setGeometry(width() / 2 - 1.25 * hexagon_size - sqrt(3) * spacing_size / 2, height() / 2 - (sqrt(3) + 2) / 4 * hexagon_size - spacing_size / 2, hexagon_size, hexagon_size);
    connect(this, SIGNAL(sendMemUsage(const int)), mem_usage_widget_ptr, SLOT(Slot_UpdateRate(const int)));

    stb_1_widget_ptr = new HexagonWidget("NULL", this);
    stb_1_widget_ptr->setGeometry((width() - hexagon_size) / 2, height() / 2 - (sqrt(3) + 1) / 2 * hexagon_size - spacing_size, hexagon_size, hexagon_size);

    stb_2_widget_ptr = new HexagonWidget("NULL", this);
    stb_2_widget_ptr->setGeometry(width() / 2 + 0.25 * hexagon_size + sqrt(3) * spacing_size / 2, height() / 2 - (sqrt(3) + 2) / 4 * hexagon_size - spacing_size / 2, hexagon_size, hexagon_size);

    stb_3_widget_ptr = new HexagonWidget("NULL", this);
    stb_3_widget_ptr->setGeometry(width() / 2 + 0.25 * hexagon_size + sqrt(3) * spacing_size / 2,  height() / 2 - (3 * sqrt(3) + 2) / 4 * hexagon_size - 1.5 * spacing_size, hexagon_size, hexagon_size);

    refresh_timer_ptr = new QTimer(this);
    connect(refresh_timer_ptr, SIGNAL(timeout()), this, SLOT(Slot_Refresh_Timeout()));
    refresh_timer_ptr->start(800);

}

MonitorWidget::~MonitorWidget()
{
    delete ui;
    delete refresh_timer_ptr;
    delete cpu_usage_widget_ptr;
    delete mem_usage_widget_ptr;
    delete stb_1_widget_ptr;
    delete stb_2_widget_ptr;
    delete stb_3_widget_ptr;
}
void MonitorWidget::paintEvent(QPaintEvent *event)
{
//    QPainter painter(this);
//    painter.setPen(Qt::white);
//    painter.drawRect(QRect(0, 0, width() -1, height() -1));
    event->accept();
}

void MonitorWidget::Slot_Refresh_Timeout()
{
    emit requestCpuUsage();
    emit requestMemUsage();
}
void MonitorWidget::Slot_CpuUsageResultReady(const int rate)
{ emit sendCpuUsage(rate); }
void MonitorWidget::Slot_MemUsageResultReady(const int rate)
{ emit sendMemUsage(rate); }
