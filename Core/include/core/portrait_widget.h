#ifndef IONADESKTOP_CORE_PORTRAIT_WIDGET_H
#define IONADESKTOP_CORE_PORTRAIT_WIDGET_H

#include <QWidget>
#include <QBitmap>
#include <QPixmap>

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
        QPixmap pix_portrait;
        QBitmap mask_portrait;
        void paintEvent(QPaintEvent *ev);
        void mousePressEvent(QMouseEvent* ev);
    };
}
}

#endif // IONADESKTOP_CORE_PORTRAIT_WIDGET_H
