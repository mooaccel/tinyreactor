//
// Created by mojiajun on 2020/3/3.
//
#ifndef TINYREACTOR_SRC_EVENTLOOP_THREAD_H_
#define TINYREACTOR_SRC_EVENTLOOP_THREAD_H_

#include <condition_variable>
#include <thread>
#include <mutex>

#include "event_loop.h"

namespace tinyreactor {

class EventLoopThread {
 public:
  EventLoopThread();
  ~EventLoopThread();
  EventLoop *getEventLoop();

 private:

  /// thread_执行的线程函数
  void threadFunc();

  EventLoop *loop_;
  // 构造函数即启动线程的执行
  std::mutex mutex_;
  std::condition_variable cond_;
  std::thread thread_;
};

}

#endif //TINYREACTOR_SRC_EVENTLOOP_THREAD_H_
