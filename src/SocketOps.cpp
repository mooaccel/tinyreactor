//
// Created by 莫佳骏 on 2019/9/12.
//

#include "SocketOps.h"

#include <cstdio>
#include <cstring>
#include <iostream>

#include <unistd.h>
#include <arpa/inet.h>  // ::inet_pton
#include <netinet/in.h>  // sockaddr_in, sockaddr
#include <fcntl.h>
#include <sys/uio.h>

using namespace monoreator;

int sockets::createSocket() {
    int socketfd = ::socket(AF_INET,
                            SOCK_STREAM,
                            0);
    if (socketfd < 0) {
        std::fprintf(stderr, "sockets::createSocket() occur error.\n");
    }
    return socketfd;
}

const struct sockaddr *sockets::sockaddr_cast(const struct sockaddr_in *addr) {
    return static_cast<const struct sockaddr *>(implicit_cast<const void *>(addr));
}

struct sockaddr *sockets::sockaddr_cast(struct sockaddr_in *addr)  // 这样重载可以吗?
{
    return static_cast<struct sockaddr *>(implicit_cast<void *>(addr));
}

const struct sockaddr_in *sockets::sockaddr_in_cast(const struct sockaddr *addr) {
    return static_cast<const struct sockaddr_in *>(implicit_cast<const void *>(addr));
}

void sockets::generateAddrfromIpPort(const char *ip, uint16_t port, struct sockaddr_in *addr) {
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    if (::inet_pton(AF_INET, ip, &addr->sin_addr) <= 0)  // populate addr->sin_addr
    {
        fprintf(stderr, "sockets::fromIpPort");
    }
}

void sockets::fromIpPort(const char *ip, uint16_t port,
                         struct sockaddr_in *addr) {
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    if (::inet_pton(AF_INET, ip, &addr->sin_addr) <= 0) {
        std::cout << "sockets::fromIpPort";
    }
}

struct sockaddr_in sockets::getLocalAddr(int sockfd) {
    struct sockaddr_in localaddr;
    ::memset(&localaddr, 0, sizeof localaddr);
    socklen_t addrlen = static_cast<socklen_t>(sizeof localaddr);
    if (::getsockname(sockfd, monoreator::sockets::sockaddr_cast(&localaddr), &addrlen) < 0) {
        std::cout << "sockets::getLocalAddr error" << '\n';
    }
    return localaddr;
}

void sockets::toIp(char *buf, size_t size,
                   const struct sockaddr *addr) {
    if (addr->sa_family == AF_INET) {
        const struct sockaddr_in *addr4 = monoreator::sockets::sockaddr_in_cast(addr);
        ::inet_ntop(AF_INET, &addr4->sin_addr, buf, static_cast<socklen_t>(size));
    } else {
        fprintf(stderr, "toIp error\n");
    }
}

void sockets::toIpPort(char *buf, size_t size,
                       const struct sockaddr *addr) {
    monoreator::sockets::toIp(buf, size, addr);
    size_t end = ::strlen(buf);
    const struct sockaddr_in *addr4 = monoreator::sockets::sockaddr_in_cast(addr);
    uint16_t port = ntohs(addr4->sin_port);
    snprintf(buf + end, size - end, ":%u", port);
}

void sockets::setNonBlockAndCloseOnExec(int sockfd) {
    // non-block
    int flags = ::fcntl(sockfd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    int ret = ::fcntl(sockfd, F_SETFL, flags);
    // FIXME check

    // close-on-exec
    flags = ::fcntl(sockfd, F_GETFD, 0);
    flags |= FD_CLOEXEC;
    ret = ::fcntl(sockfd, F_SETFD, flags);
    // FIXME check

    (void) ret;
}

ssize_t sockets::write(int sockfd, const void *buf, size_t count) {
    return ::write(sockfd, buf, count);
}

ssize_t sockets::read(int sockfd, void *buf, size_t count) {  // 暂时不用它  为啥const void *buf不行, write() 却可以?
    return ::read(sockfd, buf, count);
}

ssize_t sockets::readv(int sockfd, const struct iovec *iov, int iovcnt) {
    return ::readv(sockfd, iov, iovcnt);
}
