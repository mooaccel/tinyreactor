//
// Created by mojiajun on 2019/9/12.
//

#ifndef SRC_ACCEPTOR_H_
#define SRC_ACCEPTOR_H_

#include <functional>

class EventLoop;
class InetAddress;
class Channel;
class Socket;

class Acceptor {
 public:
  Acceptor(const Acceptor &) = delete;
  Acceptor &operator=(const Acceptor &) = delete;

  using std::function<void> NewConnectionCallback;
  Acceptor(EventLoop *loop, const InetAddress listenAddr);
  ~Acceptor();
  void listen();

 private:
  handleRead();

  // 不要EventLoop *可以吗?
  Socket listenSocket_;
  Channel listenChannel_;
  NewConnectionCallback newConnectionCallback_;
};

#endif //SRC_ACCEPTOR_H_
