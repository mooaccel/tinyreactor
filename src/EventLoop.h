#ifndef SRC_EVENTLOOP_H
#define SRC_EVENTLOOP_H

#include "Channel.h"
#include "Epoll.h"  // class Epoll为啥不行?

#include <vector>
#include <memory>

class Channel;

class EventLoop {
 public:
  EventLoop();
  ~EventLoop() = default;

  EventLoop(const EventLoop &) = delete;
  EventLoop &operator=(const EventLoop &) = delete;

  void loop();
  void updateChannel(Channel *channel);
  void removeChannel(Channel *channel);

 private:
  std::vector<Channel *> activeChannels_;
  std::unique_ptr<Epoll> poller_;  // std::unique 怎么命名好呢...
};

#endif  // SRC_EVENTLOOP_H
