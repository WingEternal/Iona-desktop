#ifndef IONA_DESKTOP_CORE_DISPLAY_VOICE_LABEL_H
#define IONA_DESKTOP_CORE_DISPLAY_VOICE_LABEL_H

#include <QGraphicsOpacityEffect>
#include <QLabel>
#include <QMediaPlayer>
#include <QPropertyAnimation>

namespace IonaDesktop {
namespace CoreDisplay {
class VoiceLabel : public QLabel {
  Q_OBJECT
 public:
  explicit VoiceLabel(QWidget* parent = nullptr);
  ~VoiceLabel();

 private:
  QGraphicsOpacityEffect* label_opacity_fx;
  QPropertyAnimation* label_animation;
  const int label_animate_duration = 100;

 private slots:
  void onPlayerPlaybackStateChanged(const QMediaPlayer::PlaybackState);
  void onAnimationFinished();
};
}  // namespace CoreDisplay
}  // namespace IonaDesktop

#endif  // IONA_DESKTOP_CORE_DISPLAY_VOICE_LABEL_H
