//
// Created by mojiajun on 2020/2/6.
//
#include "timer_queue.h"

#include <sys/timerfd.h>
#include <unistd.h>

#include <cassert>
#include <cstring>  // memset
#include <functional>
#include <memory>

#include "glog/logging.h"
#include "../event_loop.h"
#include "timer.h"
#include "time_stamp.h"
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

/// 从调用howMuchTimeFromNow开始到expiration, 时间间隔有多长
struct timespec howMuchTimeFromNow(Timestamp expiration) {
    // 先计算这个时间的us
    int64_t microseconds = expiration.microSecondsSinceEpoch()
        - Timestamp::now().microSecondsSinceEpoch();
    if (microseconds < 100) {  // ?
        microseconds = 100;
    }
    // 再转换成timespec结构体
    struct timespec ts;
    ts.tv_sec = static_cast<time_t>(
        microseconds / Timestamp::kMicroSecondsPerSecond);
    ts.tv_nsec = static_cast<long>(
        (microseconds % Timestamp::kMicroSecondsPerSecond) * 1000);
    return ts;
}

void readTimerfd(int timerfd) {
    uint64_t nums;
    ssize_t n = ::read(timerfd, &nums, sizeof nums);
    LOG(INFO) << "readTimerfd number of expiration occur "
              << nums
              << " times";
    if (n != sizeof nums) {
        LOG(FATAL) << "readTimerfd read " << n << " bytes instead of 8";
    }
}

/// 设置Timerfd的超时时间, Timer假设是周期性触发的, 也被设置为多个单次触发
/// @timerfd 被设置的timerfd
/// @expiration 超时时间点
void setTimerfdExpiration(int timerfd, Timestamp expiration) {
    struct itimerspec new_value;
    std::memset(&new_value, 0, sizeof new_value);
    new_value.it_value = howMuchTimeFromNow(expiration);
    // By default, the initial expiration time specified in new_value is
    // interpreted relative to the current time on the timer's clock at the
    // time of the call
    int ret = ::timerfd_settime(timerfd, 0, &new_value, nullptr);
    if (ret < 0) {
        LOG(FATAL) << "::timerfd_settime() fail";
    }
}

}

}

using namespace tinyreactor;

TimerQueue::TimerQueue(EventLoop *loop) :
    loop_(loop),
    timerfd_(impl::createTimerfd()),
    timerfdChannel_(std::make_unique<Channel>(loop_, timerfd_)) {
    // 让timerfdChannel_关注Readable事件
    timerfdChannel_->enableReading();
    // 设置可读时的回调函数
    timerfdChannel_->setReadCallback(
        std::bind(&TimerQueue::handleRead, this)
    );
}

TimerQueue::~TimerQueue() {
    // 关闭Channel..
    timerfdChannel_->disableAll();
    timerfdChannel_->remove();
    ::close(timerfd_);
}

TimerId TimerQueue::addTimer(TimerCallback timercb,
                             Timestamp expiration_timepoint,
                             double interval) {
    Timer *timer = new Timer(timercb, expiration_timepoint, interval);
    TimerId timerid(timer);
    loop_->runInLoop(
        std::bind(&TimerQueue::addTimerInLoop, this, timer)
    );
    return timerid;
}

void TimerQueue::addTimerInLoop(Timer *timer) {
    loop_->assertInLoopThread();
    bool isInQueueFront = insertQueue(timer);
    if (isInQueueFront) {
        tinyreactor::impl::setTimerfdExpiration(timerfd_, timer->expiration());
    }
}

void TimerQueue::cancelTimer(TimerId timerid) {

}

bool TimerQueue::insertQueue(Timer *timer) {
    loop_->assertInLoopThread();
    Timestamp timer_expiration = timer->expiration();
    bool isInQueueFront = false;
    auto it = timers_.begin();
    if (it == timers_.end() || timer_expiration < it->first) {
        isInQueueFront = true;
    }
    timers_[timer_expiration].push_back(timer);
    return isInQueueFront;
}

/// 截止now_timepoint, 把超时的Timer*取出来
std::vector<Timer *> TimerQueue::getExpiredTimer(Timestamp now_timepoint) {
    std::vector<Timer *> expired_timers;
    // expired_timers里面的元素都 < now_timepoint
    auto end = timers_.lower_bound(now_timepoint);
    // 如果now_timepoint == end->first呢
    assert(end == timers_.end() || now_timepoint < end->first);

    for (auto it = timers_.begin(); it != end; ++it) {
        for (auto it_vec = it->second.begin(); it_vec != it->second.end(); ++it_vec) {
            expired_timers.push_back(*it_vec);
        }
    }
    timers_.erase(timers_.begin(), end);
    return expired_timers;
}

void TimerQueue::handleRead() {
    loop_->assertInLoopThread();
    impl::readTimerfd(timerfd_);
    Timestamp now_timepoint = Timestamp::now();
    std::vector<Timer *> expired_timers = getExpiredTimer(now_timepoint);

    for (auto &item : expired_timers) {
        item->timer_cb();
    }

    resetTimerQueueAndTimerfd(expired_timers, now_timepoint);
}

/// reset什么? 命名得改
void TimerQueue::resetTimerQueueAndTimerfd(std::vector<Timer *> &expired_timers, Timestamp now_timepoint) {
    Timestamp nextExpired;

    // 相等的情况怎么办?
    // repeat的timer还没有考虑进来

    if (!timers_.empty()) {
        nextExpired = timers_.begin()->first;
    }
    if (nextExpired.valid()) {
        impl::setTimerfdExpiration(timerfd_, nextExpired);
    }
}
