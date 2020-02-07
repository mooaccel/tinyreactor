//
// Created by mojiajun on 2020/2/6.
//
#include "timer_queue.h"

#include <memory>
#include "timer.h"

using namespace tinyreactor;

TimerId TimerQueue::addTimer(TimerCallback timercb,
                             Timestamp when,
                             double interval) {
    std::unique_ptr<Timer> timer = std::make_unique<Timer>(timercb, when, interval_);
    TimerId timerid(timer.get());
    loop_->runInLoop() {
        std::bind(&TimerQueue::addTimerInLoop, this, std::move(timer));
    }
    return timerid;
}
