#ifndef IONADESKTOP_CORE_DISPLAY_APP_THREAD_HELPER
#define IONADESKTOP_CORE_DISPLAY_APP_THREAD_HELPER

#include <QDebug>
#include <QObject>
#include <QThread>

// Helper function
template <typename T>
struct has_manual_start {
 private:
  template <typename U>
  static constexpr auto check(int)
      -> decltype(std::declval<U>().onStart(), std::true_type());
  template <typename U>
  static constexpr std::false_type check(...);

 public:
  static constexpr bool value =
      std::is_same<decltype(check<T>(0)), std::true_type>::value;
};

template <typename T>
class ThrdCtrl {
 public:
  ThrdCtrl(QThread::Priority priority = QThread::InheritPriority)
      : instance(new T), thrd(new QThread) {
    instance->moveToThread(thrd);
    instance->register_ss();
    QObject::connect(thrd, &QThread::finished, instance, &QObject::deleteLater);
    if (has_manual_start<T>::value)
      QObject::connect(thrd, SIGNAL(started()), instance, SLOT(onStart()));
    else
      thrd->start(priority);
  }

  ~ThrdCtrl() {
    thrd->quit();
    thrd->wait();
  }

  void start(QThread::Priority priority = QThread::InheritPriority) {
    if (!thrd->isRunning())
      thrd->start(priority);
    else
      qDebug() << "thread " << thrd << "already running";
  }

 private:
  T* instance;
  QThread* thrd;
};

#endif  // IONADESKTOP_CORE_DISPLAY_APP_THREAD_HELPER
