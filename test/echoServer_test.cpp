//
// Created by mojiajun on 2019/9/12.
//

#include "../src/TcpServer.h"
#include "../src/EventLoop.h"
#include "../src/InetAddress.h"

#include <iostream>
#include <utility>

#include <stdio.h>
#include <unistd.h>

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

  void startListen()
  {
      server_.startListen();
  }
  // void stop();

 private:
  void onConnection(const TcpConnectionPtr& conn)
  {
      LOG_TRACE << conn->peerAddress().toIpPort() << " -> "
                << conn->localAddress().toIpPort() << " is "
                << (conn->connected() ? "UP" : "DOWN");
      LOG_INFO << conn->getTcpInfoString();

      conn->send("hello\n");
  }

  void onMessage(const TcpConnectionPtr& conn, Buffer* buf)
  {
      string msg(buf->retrieveAllAsString());
      LOG_TRACE << conn->name() << " recv " << msg.size() << " bytes at " << time.toString();
      if (msg == "exit\n")
      {
          conn->send("bye\n");
          conn->shutdown();
      }
      if (msg == "quit\n")
      {
          loop_->quit();
      }
      conn->send(msg);
  }

  EventLoop* loop_;
  TcpServer server_;
};

int main(int argc, char* argv[])
{
    std::cout << "sizeof TcpConnection = " << sizeof(TcpConnection);
    EventLoop loop;
    InetAddress listenAddr("127.0.0.1", 20000);
    EchoServer server(&loop, listenAddr);
    server.startListen();

    loop.loop();
}
