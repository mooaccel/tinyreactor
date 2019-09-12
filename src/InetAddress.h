//
// Created by 莫佳骏 on 2019/9/12.
//

#ifndef SRC_INETADDRESS_H_
#define SRC_INETADDRESS_H_

class InetAddress {
 public:
  InetAddress(StringArg ip, uint16_t port, bool ipv6 = false);

  void setSockAddr(const struct sockaddr_in &sockaddr4) {
      addr_ = sockaddr4;
  }
  const struct sockaddr *getSockAddr() const { return sockets::sockaddr_cast(&addr_); }

 private:
  struct sockaddr_in addr_;
};

#endif //SRC_INETADDRESS_H_
