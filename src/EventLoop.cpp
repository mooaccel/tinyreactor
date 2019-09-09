#include "EventLoop.h"

#include "Epoll.h"

#include <memory>

EventLoop::EventLoop() :
    poller(make_unique<Epoll>(this)) {
}

void EventLoop::loop() {
    poller_->poll(nullptr, activeChannels_);
    for (Channel *curChannel : activeChannels_) {
        curChannel->handleEvent();
    }
}
void EventLoop::updateChannel(Channel *channel) {
    poller_->updateChannelInEpoll(channel);
}
