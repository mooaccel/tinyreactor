//
// Created by 莫佳骏 on 2019/9/12.
//

#include "SocketOps.h"

#include <cstdio>
#include <netinet/in.h>  // sockaddr_in, sockaddr

using namespace monoreator;

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
