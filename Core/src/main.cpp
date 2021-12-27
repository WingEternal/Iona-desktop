#include <QApplication>
#include "main_widget/main_widget.h"
#include "plugin_core/plugin_manager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWidget* main_widget_ptr = new MainWidget();
    main_widget_ptr->show();
    PluginCore::PluginManager();
    return a.exec();
}
