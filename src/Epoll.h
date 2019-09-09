#include "EventLoop.h"
#include "Channel.h"

struct epoll_event;

class Epoll {
 public:
  Epoll(EventLoop *ownerLoop);
  ~Epoll();

  int poll();
  void updateChannelInEpoll(Channel *channel);
  void removeChannelInEpoll(Channel *channel);

 private:
  static const int kInitEventListSize = 16;
  EventLoop *ownerLoop_;
  int epfd_;
  std::vector<struct epoll_event> events_;  // 类里面放vector扩容问题?
};