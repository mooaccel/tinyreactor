//
// Created by 莫佳骏 on 2019/9/12.
//

#ifndef SINGLE_THREAD_REACTOR_SRC_TCPCLIENT_H_
#define SINGLE_THREAD_REACTOR_SRC_TCPCLIENT_H_

#include "connector.h"

#include <string>

namespace tinyreactor {

class EventLoop;
class InetAddress;
class Connector;

class TcpClient {
 public:
  TcpClient(EventLoop *loop,
            const InetAddress &serverAddr,
            const std::string &name);
  ~TcpClient();  // force out-line dtor, for std::unique_ptr members.
 private:
  EventLoop *loop_;
  Connector connector_;
  std::string name_;
};

}
#endif //SINGLE_THREAD_REACTOR_SRC_TCPCLIENT_H_
