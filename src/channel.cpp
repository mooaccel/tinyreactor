//
// Created by 莫佳骏 on 2019/9/9.
//

#include <sys/epoll.h>

#include "channel.h"
#include "event_loop.h"

using namespace tinyreactor;

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = EPOLLIN | EPOLLPRI;
const int Channel::kWriteEvent = EPOLLOUT;

Channel::Channel(EventLoop *loop, int fd_)
    : ownerLoop_(loop),
      fd_(fd_),
      events_(0),
      revents_(0) {
}

void Channel::handleEvent() {
    // 检测有效性.
    if (revents_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP)) {
        if (readCallback_) readCallback_();
    }
    if (revents_ & EPOLLOUT) {
        if (writeCallback_) writeCallback_();
    }
}

void Channel::update() {
    ownerLoop_->updateChannel(this);
}

void Channel::remove() {
    ownerLoop_->removeChannel(this);
}

bool Channel::isNoneEvent() const {
    return events_ == kNoneEvent;
}
