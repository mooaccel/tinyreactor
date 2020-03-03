//
// Created by mojiajun on 2020/3/3.
//
#include "eventloop_thread.h"

#include <condition_variable>
#include <thread>
#include <mutex>

using namespace tinyreactor;

EventLoopThread::EventLoopThread()
    : loop_(nullptr),
      mutex_(),  // 默认构造
      cond_(),  // 默认构造
      thread_(std::bind(&EventLoopThread::threadFunc, this)) {
}

EventLoopThread::~EventLoopThread() {
    thread_.join();
}

void EventLoopThread::threadFunc() {
    EventLoop loop;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        loop_ = &loop;
    }
    // notify_one之前必须没有持有锁
    cond_.notify_one();
    // 进入loop()循环
    loop.loop();
    //assert(exiting_);
    std::lock_guard<std::mutex> lock(mutex_);
    loop_ = nullptr;
}

/// getEventLoop()函数和EventLoopThread::threadFunc()不在同一线程执行, 所以需要条件变量
EventLoop *EventLoopThread::getEventLoop() {
    // TODO, assert(线程已经启动)
    EventLoop *eventloop_ptr = nullptr;
    {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [this] { return this->loop_ != nullptr; });
        // loop_是什么时候填充进去的?
        // 在另外一个线程中执行EventLoopThread::threadFunc()里面
        eventloop_ptr = loop_;
    }  // unlock在这里执行
    return eventloop_ptr;
}
