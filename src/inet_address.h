//
// Created by 莫佳骏 on 2019/9/12.
//

#ifndef SRC_INETADDRESS_H_
#define SRC_INETADDRESS_H_

#include <string>
#include <cstring>

#include <netinet/in.h>

class InetAddress {
 public:
  explicit InetAddress(std::string ip = "127.0.0.1", uint16_t port = 9877);

  explicit InetAddress(const struct sockaddr_in& addr)
      : addr_(addr)
  { }

  void setSockAddr(const struct sockaddr_in &sockaddr4) {
      addr_ = sockaddr4;
  }
  //const struct sockaddr *getSockAddr() const { return monoreator::sockets::sockaddr_cast(&addr_); }
  const struct sockaddr* getSockAddr() const;

  std::string toIpPort() const;

 private:
  struct sockaddr_in addr_;
};

#endif //SRC_INETADDRESS_H_
