//
// Created by 莫佳骏 on 2019/9/12.
//

#include "../src/TcpClient.h"

#include "../src/EventLoop.h"
#include "../src/InetAddress.h"

#include <iostream>
#include <utility>
#include <memory>

#include <stdio.h>
#include <unistd.h>

class EchoClient {
 public:
  EchoClient(const EchoClient &) = delete;
  EchoClient &operator=(const EchoClient &) = delete;

  EchoClient(EventLoop *loop, const InetAddress &listenAddr, const string &id)
      : loop_(loop),
        client_(loop, listenAddr, "EchoClient" + id) {
      client_.setConnectionCallback(
          std::bind(&EchoClient::onConnection, this, _1));
      client_.setMessageCallback(
          std::bind(&EchoClient::onMessage, this, _1, _2));
      //client_.enableRetry();
  }

  void connect() {
      client_.connect();
  }
  // void stop();

 private:
  void onConnection(const TcpConnectionPtr &conn) {
      std::cout << conn->localAddress().toIpPort() << " -> "
                << conn->peerAddress().toIpPort() << " is "
                << (conn->connected() ? "UP" : "DOWN");
      conn->send("client have known connection is established\n");
  }

  void onMessage(const TcpConnectionPtr &conn, Buffer *buf) {
      string msg(buf->retrieveAllAsString());
      std::cout << conn->name() << " recv " << msg.size() << " bytes at ";
      conn->send(msg);
  }

  EventLoop *loop_;
  TcpClient client_;
};

int main(int argc, char **argv) {
    if (argc == 1) {
        printf("Usage: %s host_ip", argv[0]);
    }
    EventLoop loop;
    InetAddress serverAddr(argv[1], 20000);
    char buf[32];
    snprintf(buf, sizeof buf, "%d", i + 1);
    std::unique_ptr<EchoClient> client(std::make_unique<EchoClient>(&loop, serverAddr, buf));

    client->connect();
    loop.loop();
    return 0;
}
