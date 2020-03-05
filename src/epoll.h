#ifndef SRC_EPOLL_H
#define SRC_EPOLL_H

#include <sys/epoll.h>

#include <vector>
#include <map>

namespace tinyreactor {

class EventLoop;
class Channel;

class Epoll {
 public:
  Epoll(EventLoop *ownerLoop);
  ~Epoll();

  Epoll(const Epoll &) = delete;
  Epoll &operator=(const Epoll &) = delete;

  /// 调用epoll_wait()
  void poll(struct timeval *tvp, std::vector<Channel *> &activeChannels);

  /// 在epfd_里面更新channel对应的fd, 使感兴趣的IO事件被epoll监听
  /// 必须在ownerLoop_线程里面调用
  void updateChannelInEpoll(Channel *channel);

  /// 在epfd_里面移除channel对应的fd
  /// 必须在ownerLoop_线程里面调用
  void removeChannelInEpoll(Channel *channel);

 private:
  // FIXME: 暂时用这个常量
  static const int kInitEventListSize = 16;

  EventLoop *ownerLoop_;

  int epfd_;
  // fd -> Channel*的映射
  std::map<int, Channel*>  channels_;

  //std::vector<struct epoll_event> revents_;
};

}
#endif  // SRC_EPOLL_H
