//
// Created by mojiajun on 2019/9/9.
//

#ifndef REACTOR_CPP_SRC_CHANNEL_H_
#define REACTOR_CPP_SRC_CHANNEL_H_

#include "EventLoop.h"

#include <functional>
#include <sys/epoll.h>

// Channel 负责事件分发
// 每个Channel只负责一个fd
class Channel {
 public:
  using EventCallback = std::function<void()>;

  Channel(EventLoop *loop, int fd) : loopOwner_(loop), fd_(fd) {}
  void handleEvent();

  int events() const { return events_ };
  void set_events(int events) { events_ = events; }
  int revents() const { return revents_ };
  void set_revents(int revents) { revents_ = revents };
  void set_fd(int fd) {fd_ = fd; }
  int fd() const { return fd_; }

  void enableReading() { events_ |= kReadEvent; update(); }

  void setReadCallback(EventCallback &cb) { readCallback_ = cb; }
  void setWriteCallback(EventCallback &cb) { writeCallback_ = cb; }

 private:
  const static int kNoneEvent = 0;
  const static int kReadEvent = POLLIN | POLLPRI;
  const static int kWriteEvent = POLLOUT;

  void update();

  EventLoop *loopOwner_;
  int fd_;
  int events_;
  int revents_;

  EventCallback readCallback_;
  EventCallback writeCallback_;
};

#endif //REACTOR_CPP_SRC_CHANNEL_H_
