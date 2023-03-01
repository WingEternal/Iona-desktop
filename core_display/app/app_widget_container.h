#ifndef IONADESKTOP_CORE_DISPLAY_APP_WIDGET_CONTAINER_H
#define IONADESKTOP_CORE_DISPLAY_APP_WIDGET_CONTAINER_H

#include <QWidget>

/* Description:
* Base widget of all other widgets */

/* DISPLAY INCLUDES */
#include "iona_gl/gl_widget.h"
/* VOICE INCLUDES */
#include "voice/voice_label.h"
/* TRAY MENU INCLUDES */
#include "tray/tray.h"
/* MOVE_WIDGET INCLUDES */
#include "move_ctrl/move_widget.h"
/* CMDS INCLUDES */
#include "quick_cmds/quick_cmds_entry.h"

namespace IonaDesktop {
namespace CoreDisplay {
    class AppWidgetContainer : public QWidget
    {
        Q_OBJECT
    public:
        explicit AppWidgetContainer(QWidget *parent = nullptr);
        ~AppWidgetContainer();

    private:
        GLWidget* gl_widget;
        VoiceLabel* voice_label;
        Tray* tray;
        MoveWidget* move_ctrl;
        QuickCmdsEntry* quick_cmds;

    protected:
        void paintEvent(QPaintEvent* ev);

    signals:

    public slots:
        void trayRequestHide();
        void trayRequestResetGeometry();

#ifdef Q_OS_WIN
        bool installHook();
        bool uninstallHook();
#endif
    };

}
}

#endif // IONADESKTOP_CORE_DISPLAY_APP_WIDGET_CONTAINER_H
