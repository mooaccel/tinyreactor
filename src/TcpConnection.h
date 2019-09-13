//
// Created by 莫佳骏 on 2019/9/12.
//

#ifndef SINGLE_THREAD_REACTOR_SRC_TCPCONNECTION_H_
#define SINGLE_THREAD_REACTOR_SRC_TCPCONNECTION_H_

#include "Callback.h"

#include <string>
#include <memory>

class EventLoop;
class InetAddress;
class Socket;
class Channel;

class TcpConnection {
 public:
  TcpConnection(EventLoop *loop, std::string connname, int sockfd, InetAddress *localAddr, InetAddress peerAddr);
  ~TcpConnection() = default;

  void connectEstablished();

  void setConnectionCallback(const ConnectionCallback& cb)
  { connectionCallback_ = cb; }
  void setMessageCallback(const MessageCallback& cb)
  { messageCallback_ = cb; }

 private:
  EventLoop *loop_;
  const std::string connname_;
  std::unique_ptr<Socket> socket_;  // Wrapper of connfd
  std::unique_ptr<Channel> channel_;  // Channel corresponding to connfd
  const InetAddress &localAddr_;
  const InetAddress &peerAddr_;

  ConnectionCallback connectionCallback_;
  MessageCallback messageCallback_;
};

using TcpConnectionPtr = std::shared_ptr<TcpConnection>;

#endif //SINGLE_THREAD_REACTOR_SRC_TCPCONNECTION_H_
