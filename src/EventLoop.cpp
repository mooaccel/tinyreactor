#include "EventLoop.h"

#include "Epoll.h"
#include "Channel.h"

#include <memory>

EventLoop::EventLoop() :
    poller_(std::make_unique<Epoll>(this)) {
}

void EventLoop::loop() {
    poller_->poll(nullptr, activeChannels_);
    for (auto curChannel : activeChannels_) {
        curChannel->handleEvent();
    }
}
void EventLoop::updateChannel(Channel *channel) {
    poller_->updateChannelInEpoll(channel);
}
