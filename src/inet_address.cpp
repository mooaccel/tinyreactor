//
// Created by 莫佳骏 on 2019/9/12.
//

#include "inet_address.h"
#include "socket_operations.h"

#include <string>
#include <cstring>

using namespace monoreator;

InetAddress::InetAddress(std::string ip, uint16_t port) {
    ::memset(&addr_, 0, sizeof addr_);  // 可以不用吗?
    monoreator::sockets::fromIpPort(ip.c_str(), port, &addr_);
}

std::string InetAddress::toIpPort() const {
    char buf[64] = "";
    monoreator::sockets::toIpPort(buf, sizeof buf, getSockAddr());
    return buf;
}

const struct sockaddr *InetAddress::getSockAddr() const {
    return static_cast<const struct sockaddr *>(sockets::sockaddr_cast(&addr_));
}
