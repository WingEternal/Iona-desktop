#ifndef IONADESKTOP_CORE_DISPLAY_APP_CONTENT_H
#define IONADESKTOP_CORE_DISPLAY_APP_CONTENT_H

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
class AppContent : public QWidget {
  Q_OBJECT
 public:
  explicit AppContent(QWidget* parent = nullptr);
  ~AppContent();

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

}  // namespace CoreDisplay
}  // namespace IonaDesktop

#endif  // IONADESKTOP_CORE_DISPLAY_APP_CONTENT_H
