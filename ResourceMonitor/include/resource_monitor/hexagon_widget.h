#ifndef IONADESKTOP_RM_HEXAGON_WIDGET_H
#define IONADESKTOP_RM_HEXAGON_WIDGET_H

#include <QWidget>
#include <QLabel>

namespace IonaDesktop {
namespace ResourceMonitor {
    class HexagonWidget : public QWidget
    {
        Q_OBJECT
    public:
        explicit HexagonWidget(const QString &widget_name = "NULL", QWidget *parent = nullptr);
        ~HexagonWidget();
        void setName(const QString &name);
    private:

    protected:
        int rate;
        QLabel *rate_label_ptr;
        QString name;
        QLabel *name_label_ptr;
        void paintEvent(QPaintEvent *event);
    public slots:
        void Slot_UpdateRate(const int new_rate);
    };
}
}


#endif // IONADESKTOP_RM_HEXAGON_WIDGET_H
