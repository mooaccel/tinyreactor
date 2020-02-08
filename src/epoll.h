#ifndef SRC_EPOLL_H
#define SRC_EPOLL_H

#include <vector>

namespace tinyreactor {

class EventLoop;
class Channel;
struct epoll_event;

class Epoll {
 public:
  Epoll(EventLoop *ownerLoop);
  ~Epoll();

  Epoll(const Epoll &) = delete;
  Epoll &operator=(const Epoll &) = delete;

  int poll(struct timeval *tvp, std::vector<Channel *> &activeChannels);
  void updateChannelInEpoll(Channel *channel);
  void removeChannelInEpoll(Channel *channel);

 private:
  static const int kInitEventListSize = 16;
  EventLoop *ownerLoop_;
  int epfd_;
  std::vector<struct epoll_event> events_;  // 类里面放vector扩容问题?
};

}
#endif  // SRC_EPOLL_H
