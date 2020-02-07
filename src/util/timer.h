//
// Created by mojiajun on 2020/2/7.
//
#ifndef TINYREACTOR_SRC_UTIL_TIMER_H_
#define TINYREACTOR_SRC_UTIL_TIMER_H_

#include "time_stamp.h"
#include "../callback.h"

namespace tinyreactor {

class Timer {
 public:
  Timer(const Timer &) = delete;
  Timer &operator=(const Timer &) = delete;

  Timer(TimerCallback cb, Timestamp when, double interval)
      : callback_(cb),
        expiration_(when),
        interval_(interval),
        repeat_(interval > 0.0) {
  }

  void timer_cb() {
      callback_();
  }

 private:
  const TimerCallback callback_;
  Timestamp expiration_;
  const double interval_;
  const bool repeat_;
};

}

#endif //TINYREACTOR_SRC_UTIL_TIMER_H_
