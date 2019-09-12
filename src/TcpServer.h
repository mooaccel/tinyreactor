//
// Created by mojiajun on 2019/9/12.
//

#ifndef SRC_TCPSERVER_H_
#define SRC_TCPSERVER_H_

#include <memory>

class Acceptor;

class TcpServer {
 public:
  TcpServer(const TcpServer &) = delete;
  TcpServer &operator=(const TcpServer &) = delete;
  TcpServer();
  ~TcpServer();

  void startListen() {

  }

 private:
  std::unique_ptr<Acceptor> acceptor_;

};

#endif //SRC_TCPSERVER_H_
