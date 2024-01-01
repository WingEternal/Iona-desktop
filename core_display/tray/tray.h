#ifndef IONADESKTOP_CORE_DISPLAY_TRAY_H
#define IONADESKTOP_CORE_DISPLAY_TRAY_H

#include <QAction>
#include <QSystemTrayIcon>
#include <QVector>

namespace IonaDesktop {
namespace CoreDisplay {
class Tray : public QSystemTrayIcon {
  Q_OBJECT
 public:
  Tray(QWidget* parent);

 private:
  QMenu* menu;
  QAction* app_hide;
  QAction* app_reset_geo;
  QAction* app_exit;

 private slots:
  void terminate();

  // Unused. preserve for further feature
  void trayActivated(const QSystemTrayIcon::ActivationReason& reason);
};
}  // namespace CoreDisplay
}  // namespace IonaDesktop

#endif  // IONADESKTOP_CORE_DISPLAY_TRAY_H
