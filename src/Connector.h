//
// Created by 莫佳骏 on 2019/9/12.
//

#ifndef SINGLE_THREAD_REACTOR_SRC_CONNECTOR_H_
#define SINGLE_THREAD_REACTOR_SRC_CONNECTOR_H_

class EventLoop;
class InetAddress;

class Connector {
 public:
  Connector(EventLoop *loop, const InetAddress &serverAddr);
 private:
  EventLoop *loop_;
  InetAddress serverAddr_;

};

#endif //SINGLE_THREAD_REACTOR_SRC_CONNECTOR_H_
