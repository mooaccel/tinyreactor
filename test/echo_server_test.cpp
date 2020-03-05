//
// Created by mojiajun on 2019/9/12.
//

#include "glog/logging.h"
#include "src/tcp_server.h"
#include "src/event_loop.h"
#include "src/inet_address.h"
#include "src/tcp_connection.h"

#include <iostream>
#include <utility>

#include <stdio.h>
#include <unistd.h>

using std::placeholders::_1;
using std::placeholders::_2;

using namespace tinyreactor;

class EchoServer {
 public:
  EchoServer(EventLoop *loop, const InetAddress &listenAddr)
      : loop_(loop),
        server_(loop, listenAddr, "EchoServer") {
      server_.setConnectionCallback(
          std::bind(&EchoServer::onConnection, this, _1));
      server_.setMessageCallback(
          std::bind(&EchoServer::onMessage, this, _1, _2));
  }
  ~EchoServer() = default;

  void startListen() {
      server_.startListen();
  }
  // void stop();

 private:
  // 建立连接之后的回调函数, conn是std::shared_ptr<TcpConnection>的引用
  void onConnection(const TcpConnectionPtr &conn) {
      //std::cout << conn->peerAddress().toIpPort() << " -> "
      //          << conn->localAddress().toIpPort() << " is "
      //          << (conn->connected() ? "UP" : "DOWN");
      //std::cout << conn->getTcpInfoString();

      conn->send("hello from server\n");
  }

  // 怎么确保回调收上来的是完整的数据报文?
  void onMessage(const TcpConnectionPtr &conn, Buffer *buf) {
      std::string msg(buf->retrieveAllAsString());
      std::cout << msg << '\n';
      std::cout << conn->conname() << " recv " << msg.size() << " bytes" << '\n';
      //if (msg == "exit\n")
      //{
      //    conn->send("bye\n");
      //    conn->shutdown();
      //}
      //if (msg == "quit\n")
      //{
      //    loop_->quit();
      //}
      conn->send(msg);
  }

  EventLoop *loop_;
  TcpServer server_;
};

int main(int argc, char *argv[]) {
    ::google::InitGoogleLogging(argv[0]);
    EventLoop loop;
    InetAddress listenAddr("127.0.0.1", 9878);
    EchoServer server(&loop, listenAddr);
    server.startListen();

    loop.loop();
}
