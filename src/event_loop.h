#ifndef SRC_EVENTLOOP_H
#define SRC_EVENTLOOP_H

#include "channel.h"
#include "epoll.h"  // class Epoll为啥不行?
#include "util/time_stamp.h"

#include <vector>
#include <memory>
#include <thread>

class Channel;

class EventLoop {
 public:
  using Functor = std::function<void()>;
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
  void cancelTimer();

  void runInLoop(Functor functor);
  void queueInLoop(Functor functor);

  bool isInLoopThread();
  std::thread::id getThreadIdBelongTo() { return threadIdBelongTo_; }

 private:
  std::vector<Channel *> activeChannels_;
  std::unique_ptr<Epoll> poller_;  // std::unique 怎么命名好呢...

  std::thread::id threadIdBelongTo_;

  // EventLoop拥有Timerqueue
  // std::unique_ptr<TimerQueue> timerqueue_;
};

#endif  // SRC_EVENTLOOP_H