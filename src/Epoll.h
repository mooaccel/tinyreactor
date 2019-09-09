#ifndef EPOLL_H
#define EPOLL_H

#include "EventLoop.h"

#include <vector>

struct epoll_event;
class Channel;

class Epoll {
 public:
  Epoll(EventLoop *ownerLoop);
  ~Epoll();

  int poll(struct timeval *tvp, std::vector<Channel> &activeChannels);
  void updateChannelInEpoll(Channel *channel);
  void removeChannelInEpoll(Channel *channel);

 private:
  static const int kInitEventListSize = 16;
  EventLoop *ownerLoop_;
  int epfd_;
  std::vector<struct epoll_event> events_;  // 类里面放vector扩容问题?
};

#endif
