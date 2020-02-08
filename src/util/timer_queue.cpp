//
// Created by mojiajun on 2020/2/6.
//
#include "timer_queue.h"

#include <sys/timerfd.h>
#include <unistd.h>

#include <memory>

#include "glog/logging.h"
#include "timer.h"
#include "timer_id.h"

namespace tinyreactor {

namespace impl {

int createTimerfd() {
    // CLOCK_MONOTONIC?
    // FD_CLOEXEC?有何用?
    int timerfd = ::timerfd_create(CLOCK_MONOTONIC,
                                   TFD_NONBLOCK | TFD_CLOEXEC);
    if (timerfd < 0) {
        LOG(FATAL) << "Failed in timerfd_create";
    }
    return timerfd;
}

}

}

using namespace tinyreactor;

TimerQueue::TimerQueue(EventLoop *loop) :
    loop_(loop),
    timerfd_(impl::createTimerfd()),
    timerfdChannel_(std::make_unique<Channel>(loop_, timerfd_)) {}

TimerQueue::~TimerQueue() {
    // 关闭Channel..
    timerfdChannel_->disableAll();
    timerfdChannel_->remove();
    ::close(timerfd_);
}

TimerId TimerQueue::addTimer(TimerCallback timercb,
                             Timestamp when,
                             double interval) {
    std::unique_ptr<Timer> timer = std::make_unique<Timer>(timercb, when, interval);
    TimerId timerid(timer.get());
    loop_->runInLoop()
    {
        std::bind(&TimerQueue::addTimerInLoop, this, std::move(timer));
    }
    return timerid;
}

void TimerQueue::addTimerInLoop(Timer *timer) {

}

void TimerQueue::cancelTimer(TimerId timerid) {

}
