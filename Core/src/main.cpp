#include <QApplication>
#include "core/main_widget.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(resource);
    QApplication a(argc, argv);
    IonaDesktop::Core::MainWidget *main_widget_ptr = new IonaDesktop::Core::MainWidget();
    main_widget_ptr->show();
    return a.exec();
}
