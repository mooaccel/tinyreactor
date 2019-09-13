//
// Created by mojiajun on 2019/9/12.
//

#include "../src/TcpServer.h"
#include "../src/EventLoop.h"
#include "../src/InetAddress.h"
#include "../src/TcpConnection.h"

#include <iostream>
#include <utility>

#include <stdio.h>
#include <unistd.h>

using std::placeholders::_1;
using std::placeholders::_2;

class EchoServer
{
 public:
  EchoServer(EventLoop* loop, const InetAddress& listenAddr)
      : loop_(loop),
        server_(loop, listenAddr, "EchoServer")
  {
      server_.setConnectionCallback(
          std::bind(&EchoServer::onConnection, this, _1));
      server_.setMessageCallback(
          std::bind(&EchoServer::onMessage, this, _1, _2));
  }
  ~EchoServer() = default;

  void startListen()
  {
      server_.startListen();
  }
  // void stop();

 private:
  void onConnection(const TcpConnectionPtr& conn)
  {
      //std::cout << conn->peerAddress().toIpPort() << " -> "
      //          << conn->localAddress().toIpPort() << " is "
      //          << (conn->connected() ? "UP" : "DOWN");
      //std::cout << conn->getTcpInfoString();

      conn->send("hello from server\n");
  }

  void onMessage(const TcpConnectionPtr& conn, Buffer* buf)
  {
      std::string msg(buf->buffer_);
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

  EventLoop* loop_;
  TcpServer server_;
};

int main(int argc, char* argv[])
{
    std::cout << "sizeof TcpConnection = " << sizeof(TcpConnection);
    EventLoop loop;
    InetAddress listenAddr("127.0.0.1", 9877);
    EchoServer server(&loop, listenAddr);
    server.startListen();

    loop.loop();
}
