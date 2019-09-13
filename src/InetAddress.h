//
// Created by 莫佳骏 on 2019/9/12.
//

#ifndef SRC_INETADDRESS_H_
#define SRC_INETADDRESS_H_

#include "SocketOps.h"

#include <string>
#include <cstring>

class InetAddress {
 public:
  InetAddress(std::string ip, uint16_t port) {
      ::memset(&addr_, 0, sizeof addr_);  // 可以不用吗?
      monoreator::sockets::fromIpPort(ip.c_str(), port, &addr_);
  }

  void setSockAddr(const struct sockaddr_in &sockaddr4) {
      addr_ = sockaddr4;
  }
  const struct sockaddr *getSockAddr() const { return sockets::sockaddr_cast(&addr_); }

 private:
  struct sockaddr_in addr_;
};

#endif //SRC_INETADDRESS_H_
