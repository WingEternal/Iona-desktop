#ifndef IONADESKTOP_CORE_PORTRAIT_WIDGET_H
#define IONADESKTOP_CORE_PORTRAIT_WIDGET_H

#include <QWidget>

namespace IonaDesktop {
namespace Core {
    class PortraitWidget : public QWidget
    {
        Q_OBJECT
    public:
        explicit PortraitWidget(QWidget *parent = nullptr);
        ~PortraitWidget();
    private:
    signals:
        void mousePress();
    protected:
        void paintEvent(QPaintEvent *ev);
        void mousePressEvent(QMouseEvent* ev);
    };
}
}

#endif // IONADESKTOP_CORE_PORTRAIT_WIDGET_H
