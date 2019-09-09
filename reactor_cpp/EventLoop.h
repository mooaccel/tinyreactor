#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <vector>
#include <memory>

class Channel;
class Epoll;

class EventLoop {
 public:
  EventLoop();
  void loop();
  void updateChannel(Channel *channel);

 private:
  std::vector<Channel*> activeChannels_;
  std::unique_ptr<Epoll> poller_;  // std::unique 怎么命名好呢...
};

#endif
