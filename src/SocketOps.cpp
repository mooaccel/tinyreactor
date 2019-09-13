//
// Created by 莫佳骏 on 2019/9/12.
//

#include "SocketOps.h"

#include <cstdio>
#include <netinet/in.h>  // sockaddr_in, sockaddr

using namespace monoreator;
using namespace monoreator::sockets;

int createSocket() {
    int socketfd = ::socket(AF_INET,
                            SOCK_STREAM,
                            0);
    if (socketfd < 0) {
        std::fprintf(stderr, "sockets::createSocket() occur error.\n");
    }
    return socketfd;
}

const struct sockaddr *sockaddr_cast(const struct sockaddr_in *addr_in) {
    return static_cast<const struct sockaddr *>(addr_in);
}

const struct sockaddr_in *sockaddr_in_cast(const struct sockaddr *addr) {
    return static_cast<const struct sockaddr_in *>(addr);
}

void generateAddrfromIpPort(const char *ip, uint16_t port, struct sockaddr_in *addr) {
    addr->sin_family = AF_INET;
    addr->sin_port = ::htons(port);
    if (::inet_pton(AF_INET, ip, &addr->sin_addr) <= 0)  // populate addr->sin_addr
    {
        fprintf(stderr, "sockets::fromIpPort");
    }
}
