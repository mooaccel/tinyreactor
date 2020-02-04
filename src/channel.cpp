//
// Created by 莫佳骏 on 2019/9/9.
//

#include "channel.h"
#include "event_loop.h"

#include <sys/epoll.h>

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = EPOLLIN | EPOLLPRI;
const int Channel::kWriteEvent = EPOLLOUT;

void Channel::handleEvent() {
    // 检测有效性.
    if (revents_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP))
    {
        if (readCallback_) readCallback_();
    }
    if (revents_ & EPOLLOUT)
    {
        if (writeCallback_) writeCallback_();
    }
}

void Channel::update() {
    loopOwner_->updateChannel(this);
}

void Channel::remove() {
    loopOwner_->removeChannel(this);
}
