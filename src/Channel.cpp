//
// Created by 莫佳骏 on 2019/9/9.
//

#include "Channel.h"

void Channel::handleEvent() {
    // 检测有效性.
    if (revents_ & (POLLIN | POLLPRI | POLLRDHUP))
    {
        if (readCallback_) readCallback_();
    }
    if (revents_ & POLLOUT)
    {
        if (writeCallback_) writeCallback_();
    }
}

void Channel::update() {
    loopOwner_->updateChannel(this);
}