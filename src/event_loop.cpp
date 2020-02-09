#include "event_loop.h"

#include <unistd.h>

#include <cstdio>
#include <iostream>
#include <memory>
#include <mutex>

#include "channel.h"
#include "epoll.h"
#include "util/timer_queue.h"

using namespace tinyreactor;

EventLoop::EventLoop() :
    poller_(std::make_unique<Epoll>(this)),
    timerqueue_(std::make_unique<TimerQueue>(this)),
    threadIdBelongTo_(std::this_thread::get_id()) {
}

void EventLoop::loop() {
    while (true) {
        activeChannels_.clear();
        poller_->poll(nullptr, activeChannels_);
        std::printf("poller->poll return\n");
        for (auto curChannel : activeChannels_) {
            curChannel->handleEvent();
        }
    }
}

void EventLoop::updateChannel(Channel *channel) {
    poller_->updateChannelInEpoll(channel);
}

void EventLoop::removeChannel(Channel *channel) {
    poller_->removeChannelInEpoll(channel);
}

TimerId EventLoop::runAt(Timestamp expiration_timepoint, TimerCallback timercb) {
    return timerqueue_->addTimer(std::move(timercb), expiration_timepoint, 0.0);
}

TimerId EventLoop::runAfterNow(double delay, TimerCallback timercb) {
    Timestamp expiration_timepoint = Timestamp::addTime(Timestamp::now(), delay);
    return timerqueue_->addTimer(std::move(timercb), expiration_timepoint, 0.0);
}

TimerId EventLoop::runEvery() {
    // TODO
}

void EventLoop::cancelTimer() {
    // TODO
}

void EventLoop::runInLoop(Functor functor) {
    if (isInLoopThread()) {
        functor();
    } else {
        queueInLoop(std::move(functor));
    }
}

/// 也有可能在loop thread里面调用吗?
void EventLoop::queueInLoop(Functor functor) {
    {
        std::lock_guard<std::mutex> lg(mutex_);
        pendingFunctors_.push_back(functor);
    }
    // 如果不在loop thread
    if (!isInLoopThread()) {
        wakeupLoopThread();
    }
}

void EventLoop::assertInLoopThread() {
    // 如果不在LoopThread中,那么停止这个线程
    if (!isInLoopThread()) {
        std::terminate();
    }
}

void EventLoop::wakeupLoopThread() {
    uint64_t one = 1;  // ?
    ssize_t n = ::write(wakeupFd_, &one, sizeof one);
    if (n != sizeof one) {
        std::cout << "wakeupLoopThread fail" << '\n';
    }
}

bool EventLoop::isInLoopThread() {
    return threadIdBelongTo_ == std::this_thread::get_id();
}
