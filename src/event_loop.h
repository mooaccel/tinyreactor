#ifndef SRC_EVENTLOOP_H
#define SRC_EVENTLOOP_H

#include "channel.h"
#include "epoll.h"  // class Epoll为啥不行?
#include "util/time_stamp.h"

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

  void runAt();
  void runAfter();
  void runEvery();

 private:
  std::vector<Channel *> activeChannels_;
  std::unique_ptr<Epoll> poller_;  // std::unique 怎么命名好呢...

  // EventLoop拥有Timerqueue
  // std::unique_ptr<TimerQueue> timerqueue_;
};

#endif  // SRC_EVENTLOOP_H
