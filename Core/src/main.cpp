#include <QApplication>
#include "core/base_widget.h"

using namespace IonaDesktop::Core;

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(resources);
    QApplication a(argc, argv);
    BaseWidget *base_widget_ptr = new BaseWidget();
    base_widget_ptr->show();
    return a.exec();
}
