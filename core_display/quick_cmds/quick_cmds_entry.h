#ifndef IONADESKTOP_CORE_DISPLAY_QUICK_CMDS_ENTRY_H
#define IONADESKTOP_CORE_DISPLAY_QUICK_CMDS_ENTRY_H

#include <QWidget>

namespace IonaDesktop {
namespace CoreDisplay {
    class QuickCmdsEntry : public QWidget
    {
        Q_OBJECT
    public:
        explicit QuickCmdsEntry(QWidget *parent);
        ~QuickCmdsEntry();

    private:
        QPixmap icon;

    protected:
        void paintEvent(QPaintEvent* ev) override;
        void mousePressEvent(QMouseEvent *ev) override;
        void mouseMoveEvent(QMouseEvent *ev) override;
        void mouseReleaseEvent(QMouseEvent *ev) override;
    signals:

    public slots:

    };
}
}


#endif // IONADESKTOP_CORE_DISPLAY_QUICK_CMDS_ENTRY_H
