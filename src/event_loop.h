#ifndef SRC_EVENTLOOP_H
#define SRC_EVENTLOOP_H

#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include "channel.h"
#include "epoll.h"  // class Epoll为啥不行?
#include "util/time_stamp.h"
#include "util/timer_queue.h"

namespace tinyreactor {

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
  void wakeupLoopThread();

  bool isInLoopThread();
  std::thread::id getThreadIdBelongTo() { return threadIdBelongTo_; }
  void assertInLoopThread();

 private:
  std::vector<Channel *> activeChannels_;
  std::unique_ptr<Epoll> poller_;  // std::unique 怎么命名好呢...
  std::unique_ptr<TimerQueue> timerqueue_;

  std::thread::id threadIdBelongTo_;
  int wakeupFd_;
  std::unique_ptr<Channel> wakeupChannel_;

  std::mutex mutex_;
  std::vector<Functor> pendingFunctors_;

  // EventLoop拥有Timerqueue
  // std::unique_ptr<TimerQueue> timerqueue_;
};

}
#endif  // SRC_EVENTLOOP_H
