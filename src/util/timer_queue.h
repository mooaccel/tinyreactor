//
// Created by mojiajun on 2020/2/6.
//
#ifndef TINYREACTOR_SRC_UTIL_TIMER_QUEUE_H_
#define TINYREACTOR_SRC_UTIL_TIMER_QUEUE_H_

#include "../channel.h"
#include "../callback.h"
#include "time_stamp.h"
#include "TimerId.h"

namespace tinyreactor {

class TimerQueue {
 public:
  TimerQueue(const TimerQueue &) = delete;
  TimerQueue &operator=(const TimerQueue &) = delete;

  explicit TimerQueue();
  ~TimerQueue();

  /// timercb
  /// when ???
  /// interval
  TimerId addTimer(TimerCallback timercb,
                Timestamp when,
                double interval);

  void cancelTimer() {}

 private:
  /// 每个TimerQueue一个timerfd
  const int timerfd_;
  Channel timerfdChannel_;

  // 谁持有TimerQueue
  EventLoop *loop_;

};

}

#endif //TINYREACTOR_SRC_UTIL_TIMER_QUEUE_H_
