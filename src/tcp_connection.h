//
// Created by 莫佳骏 on 2019/9/12.
//

#ifndef SINGLE_THREAD_REACTOR_SRC_TCPCONNECTION_H_
#define SINGLE_THREAD_REACTOR_SRC_TCPCONNECTION_H_

#include "buffer.h"
#include "callback.h"

#include <any>
#include <string>
#include <memory>
#include <vector>

namespace tinyreactor {

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

  /// 提供给用户从TcpConnection发送字节流的接口
  void send(const std::string &message);
  void send(const void *data, size_t len);
  /// 把buf里的readable范围内的字节发送出去
  void send(Buffer* buf);

  void connectEstablished();
  void handleRead();
  void handleWrite();
  const std::string &conname() const {
      return connname_;
  }
  bool isConnected() const { return state_ == kConnected; }
  bool isDisconnected() const { return state_ == kDisconnected; }

  void setContext(const std::any &context) { tcp_conn_ctx_ = context; }

  const std::any &getContext() const { return tcp_conn_ctx_; }

  std::any *getMutableContext() { return &tcp_conn_ctx_; }

  void setConnectionCallback(const ConnectionCallback &cb) { connectionCallback_ = cb; }
  void setMessageCallback(const MessageCallback &cb) { messageCallback_ = cb; }

 private:
  /// 从TcpConnection这个角度看Tcp的连接状态
  enum StateE { kDisconnected, kConnecting, kConnected, kDisconnecting };

  /// TcpConnection的一系列send函数最终要调用sendInLoop
  /// 需要确保线程安全
  void sendInLoop(const std::string &message);
  void sendInLoop(const void *data, size_t len);

  void setState(StateE state) { state_ = state; }

  EventLoop *ioloop_;
  const std::string connname_;
  /// 怎么确保这个状态的修改线程安全?
  StateE state_;
  std::unique_ptr<Socket> socket_;  // Wrapper of connfd
  std::unique_ptr<Channel> channel_;  // Channel corresponding to connfd
  const InetAddress &localAddr_;
  const InetAddress &peerAddr_;

  ConnectionCallback connectionCallback_;
  MessageCallback messageCallback_;
  Buffer inputBuffer_;
  Buffer outputBuffer_;
  std::any tcp_conn_ctx_;
};

using TcpConnectionPtr = std::shared_ptr<TcpConnection>;

}
#endif //SINGLE_THREAD_REACTOR_SRC_TCPCONNECTION_H_
