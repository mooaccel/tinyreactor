#ifndef SRC_EVENTLOOP_H
#define SRC_EVENTLOOP_H

#include <vector>
#include <memory>
#include "Channel.h"

class Channel;
class Epoll;

class EventLoop {
 public:
  EventLoop();
  void loop();
  void updateChannel(Channel *channel);
  void removeChannel(Channel *channel);

 private:
  std::vector<Channel*> activeChannels_;
  std::unique_ptr<Epoll> poller_;  // std::unique 怎么命名好呢...
};

#endif  // SRC_EVENTLOOP_H
