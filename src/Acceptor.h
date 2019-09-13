//
// Created by mojiajun on 2019/9/12.
//

#ifndef SRC_ACCEPTOR_H_
#define SRC_ACCEPTOR_H_

#include "Channel.h"
#include "Socket.h"

#include <functional>

class EventLoop;
class InetAddress;

class Acceptor {
 public:
  using NewConnectionCallback = std::function<void(int connfd, const InetAddress &peerAddr)>;

  Acceptor(const Acceptor &) = delete;
  Acceptor &operator=(const Acceptor &) = delete;

  Acceptor(EventLoop *loop, const InetAddress &listenAddr);
  ~Acceptor();
  void listen();
  void setNewConnectionCallback(const NewConnectionCallback &cb) {
      newConnectionCallback_ = cb;
  }

 private:
  void handleRead();

  // 不要EventLoop *可以吗? 不行吧...
  EventLoop *loop_;
  Socket listenSocket_;
  Channel listenChannel_;
  NewConnectionCallback newConnectionCallback_;
};

#endif //SRC_ACCEPTOR_H_
