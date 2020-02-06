//
// Created by mojiajun on 2020/2/6.
//
#ifndef TINYREACTOR_SRC_UTIL_TIMER_QUEUE_H_
#define TINYREACTOR_SRC_UTIL_TIMER_QUEUE_H_

#include "../channel.h"

namespace tinyreactor {

class TimerQueue {
 public:
  explicit TimerQueue();
  ~TimerQueue();

  /// timercb
  /// when ???
  /// interval
  void addTimer(TimerCallback timercb,
                Timestamp when,
                double interval);

 private:
  /// 每个TimerQueue一个timerfd
  const int timerfd_;
  Channel timerfdChannel_;
};

}

#endif //TINYREACTOR_SRC_UTIL_TIMER_QUEUE_H_
