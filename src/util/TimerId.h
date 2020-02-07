//
// Created by mojiajun on 2020/2/7.
//
#ifndef TINYREACTOR_SRC_UTIL_TIMERID_H_
#define TINYREACTOR_SRC_UTIL_TIMERID_H_

#include "timer.h"

namespace tinyreactor {

class TimerId {
 public:
  TimerId() : timer_(nullptr) {
  }

  TimerId(Timer *timer) : timer_(timer) {
  }

 private:
  Timer* timer_;
};

}

#endif //TINYREACTOR_SRC_UTIL_TIMERID_H_
