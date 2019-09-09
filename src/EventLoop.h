#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "Epoll.h"

#include <vector>
#include <memory>

class EventLoop {
 public:
  EventLoop();
  void loop();
  void updateChannel(Channel *channel);

 private:
  vector<Channel*> activeChannels_;
  std::unique_ptr<Epoll> poller_;  // std::unique 怎么命名好呢...
};

#endif