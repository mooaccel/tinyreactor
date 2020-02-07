#include "event_loop.h"

#include "epoll.h"
#include "channel.h"

#include <memory>
#include <cstdio>

EventLoop::EventLoop() :
    poller_(std::make_unique<Epoll>(this)),
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

void EventLoop::runInLoop(Functor functor) {

}

void EventLoop::queueInLoop(Functor functor) {

}


bool EventLoop::isInLoopThread() {
    return threadIdBelongTo_ == std::this_thread::get_id();
}
