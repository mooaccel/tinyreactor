#include "epoll.h"

#include <sys/time.h>
#include <sys/epoll.h>
#include <unistd.h>

#include <cassert>
#include <cstdio>
#include <cstring>
#include <map>
#include <vector>

#include "glog/logging.h"
#include "channel.h"
#include "event_loop.h"

using namespace tinyreactor;

Epoll::Epoll(EventLoop *ownerLoop) :
    ownerLoop_(ownerLoop),
    epfd_(::epoll_create1(EPOLL_CLOEXEC)) {
    //revents_(kInitEventListSize) {
    /* 1024 is just an hint for the kernel */ // from redis
    // events_, epoll_wait()第二个参数, will contain the events that will be available for the caller.
    // 用来存放epoll_wait()的结果的.
    LOG(INFO) << "In Epoll::Epoll() ctor, epfd_ = " << epfd_;
}

Epoll::~Epoll() {
    ::close(epfd_);
}

void Epoll::poll(struct timeval *tvp, std::vector<Channel *> &activeChannels) {
    // 必须是在ownerLoop中执行poll函数
    ownerLoop_->assertInLoopThread();
    for (auto it = channels_.begin(); it != channels_.end(); ++it) {
        LOG(INFO) << it->first << ' ' << it->second;
    }
    epoll_event revents[kInitEventListSize] = {0};
    int retval = ::epoll_wait(epfd_,
                              revents,
                              kInitEventListSize, // epoll_wait() 第三个参数怎么办?
                              -1);
    LOG(INFO) << "In ::epoll_wait(), return return val is " << retval;
    if (retval > 0) {
        for (int i = 0; i < retval; ++i) {
            LOG(INFO) << "revents[i].data.fd = " << revents[i].data.fd;
            auto iter = channels_.find(revents[i].data.fd);
            // 必须在channels_里存在revents[i].data.fd及其对应的value
            assert(iter != channels_.end());
            Channel *channel = iter->second;
            assert(channel->fd() == revents[i].data.fd);
            // 设置与channel有关的fd的哪些事件已经准备好了
            LOG(INFO) << "revents[i].events = " << revents[i].events ;
            channel->set_revents(revents[i].events);
            activeChannels.push_back(channel);
        }
    } else if (retval == 0) {
        LOG(INFO) << "epoll_wait() nothing happened\n";
    } else {
        LOG(ERROR) << "epoll_wait() error\n";
    }
}

void Epoll::updateChannelInEpoll(Channel *channel) {
    //ownerLoop_->assertInLoopThread();
    // 如果channel里面没有关注的事件
    //if (channel->isNoneEvent()) {
    //    removeChannelInEpoll(channel);
    //    return;
    //}
    LOG(INFO) << "Epoll::updateChannelInEpoll EventLoop " << ownerLoop_
              << " fd = " << channel->fd()
              << " events = " << channel->events();
    // 创建epoll_event结构体
    struct epoll_event event;
    std::memset(&event, 0, sizeof event);
    // 可以部分事件ET, 部分事件LT吗?
    event.events = channel->events() | EPOLLET;
    int fd_associate_channel = channel->fd();
    event.data.fd = fd_associate_channel;
    LOG(INFO) << "event.data.fd " << event.data.fd << '\n';

    int ret;
    // 如果channel里的fd已经在epfd里面监听了, 即被Epoll类记录下来了
    // 那么只是更新, 用EPOLL_CTL_MOD
    if (channels_.find(channel->fd()) != channels_.end()) {
        ret = epoll_ctl(epfd_, EPOLL_CTL_MOD, fd_associate_channel, &event);
    } else {
        ret = epoll_ctl(epfd_, EPOLL_CTL_ADD, fd_associate_channel, &event);
        // 记录下fd到channel的
        channels_[fd_associate_channel] = channel;
        assert(channels_.find(fd_associate_channel) != channels_.end());
    }
    if (ret < 0) {
        // FIXME: 处理errno
        LOG(ERROR) << "epoll_ctl occur error in Epoll::updateChannelInEpoll()\n";
        // 应该终止吧?
    }
}

/// 在epfd_里面移除channel对应的fd
void Epoll::removeChannelInEpoll(Channel *channel) {
    //ownerLoop_->assertInLoopThread();
    int fd_associate_channel = channel->fd();
    // 如果没找到fd_associate_channel对应的channel, 那么就不需要删除
    if (channels_.find(fd_associate_channel) == channels_.end()) {
        return;
    }
    LOG(INFO) << "Epoll::removeChannelInEpoll EventLoop " << ownerLoop_
              << " fd = " << channel->fd()
              << " events = " << channel->events();
    int ret = epoll_ctl(epfd_, EPOLL_CTL_DEL, fd_associate_channel, nullptr);
    size_t n = channels_.erase(channel->fd());
    assert(n == 1);
    if (ret < 0) {
        // FIXME: 处理errno
        LOG(ERROR) << "epoll_ctl occur error in Epoll::removeChannelInEpoll()\n";
        // 应该终止吧?
    }
}
