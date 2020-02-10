//
// Created by mojiajun on 2020/2/7.
//
#include "timer.h"

using namespace tinyreactor;

void Timer::restartTimer(Timestamp now) {
    // 计算下一次expiration
    if (repeat_) {
        expiration_ = Timestamp::addTime(now, interval_);
    } else {
        expiration_ = Timestamp::invalid();
    }
}
