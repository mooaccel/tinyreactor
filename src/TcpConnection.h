//
// Created by 莫佳骏 on 2019/9/12.
//

#ifndef SINGLE_THREAD_REACTOR_SRC_TCPCONNECTION_H_
#define SINGLE_THREAD_REACTOR_SRC_TCPCONNECTION_H_

#include "Buffer.h"
#include "Callback.h"

#include <string>
#include <memory>
#include <vector>

class EventLoop;
class InetAddress;
class Socket;
class Channel;
class Buffer;

class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
 public:
  TcpConnection(EventLoop *loop,
                               std::string connname,
                               int connfd,
                               const InetAddress &localAddr,
                               const InetAddress &peerAddr);
  ~TcpConnection() = default;

  void send(const std::string &message);
  void connectEstablished();
  void handleRead();
  const std::string &conname() const {
      return connname_;
  }

  void setConnectionCallback(const ConnectionCallback &cb) { connectionCallback_ = cb; }
  void setMessageCallback(const MessageCallback &cb) { messageCallback_ = cb; }

 private:
  EventLoop *loop_;
  const std::string connname_;
  std::unique_ptr<Socket> socket_;  // Wrapper of connfd
  std::unique_ptr<Channel> channel_;  // Channel corresponding to connfd
  const InetAddress &localAddr_;
  const InetAddress &peerAddr_;

  ConnectionCallback connectionCallback_;
  MessageCallback messageCallback_;
  Buffer inputBuffer_;
};

using TcpConnectionPtr = std::shared_ptr<TcpConnection>;

#endif //SINGLE_THREAD_REACTOR_SRC_TCPCONNECTION_H_
