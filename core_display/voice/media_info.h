#ifndef IONADESKTOP_CORE_DISPLAY_MEDIA_INFO_H
#define IONADESKTOP_CORE_DISPLAY_MEDIA_INFO_H

#include <QString>

namespace IonaDesktop {
namespace CoreDisplay {
class MediaInfoArray {
 private:
  struct MediaInfo {
    QString file;
    QString text;
  };

 public:
  static const MediaInfo start_info[1];
  static const MediaInfo random_info[3];
  static const MediaInfo chime_info[24];
};
}  // namespace CoreDisplay
}  // namespace IonaDesktop

#endif  // IONADESKTOP_CORE_DISPLAY_MEDIA_INFO_H
