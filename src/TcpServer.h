//
// Created by mojiajun on 2019/9/12.
//

#ifndef SRC_TCPSERVER_H_
#define SRC_TCPSERVER_H_

#include "Callback.h"
#include "Acceptor.h"  // 定义std::unique<Acceptor> acceptor_;需要

#include <functional>
#include <map>
#include <memory>
#include <string>

class EventLoop;
class InetAddress;
class Buffer;

class TcpServer {
 public:
  TcpServer(const TcpServer &) = delete;
  TcpServer &operator=(const TcpServer &) = delete;
  TcpServer(EventLoop *loop, const InetAddress &listenAddr, std::string servername);
  ~TcpServer() = default;

  void startListen() {
      acceptor_->listen();
  }
  void newConnection(int connfd, const InetAddress &peerAddr);
  void setConnectionCallback(const ConnectionCallback &cb) {
      connectionCallback_ = cb;
  }
  void setMessageCallback(const MessageCallback &cb) {
      messageCallback_ = cb;
  }

 private:
  EventLoop *loop_;
  const std::string listenIpPort_;
  const std::string servername_;
  int nextConnId_;

  std::unique_ptr<Acceptor> acceptor_;  // 直接用Acceptor不行吗?
  ConnectionCallback connectionCallback_;
  MessageCallback messageCallback_;

  std::map<std::string, TcpConnectionPtr> connections_;
};

#endif //SRC_TCPSERVER_H_
