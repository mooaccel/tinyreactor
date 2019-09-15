//
// Created by 莫佳骏 on 2019/9/12.
//

#ifndef SINGLE_THREAD_REACTOR_SRC_SOCKETOPS_H_
#define SINGLE_THREAD_REACTOR_SRC_SOCKETOPS_H_

#include <cstdint>
#include <netinet/in.h>

namespace monoreator {

namespace sockets {

int createSocket();

void generateAddrfromIpPort(const char *ip, uint16_t port, struct sockaddr_in *addr);
const struct sockaddr *sockaddr_cast(const struct sockaddr_in *addr);
struct sockaddr *sockaddr_cast(struct sockaddr_in *addr);
const struct sockaddr_in *sockaddr_in_cast(const struct sockaddr *addr);

void fromIpPort(const char *ip, uint16_t port,
                struct sockaddr_in *addr);

void toIp(char *buf, size_t size,
          const struct sockaddr *addr);

void toIpPort(char *buf, size_t size,
              const struct sockaddr *addr);

struct sockaddr_in getLocalAddr(int sockfd);

void setNonBlockAndCloseOnExec(int sockfd);
ssize_t read(int sockfd, void *buf, size_t count);
ssize_t write(int sockfd, const void *buf, size_t count);
ssize_t readv(int sockfd, const struct iovec *iov, int iovcnt);

template<typename To, typename From>
inline To implicit_cast(From const &f) {  // ?
    return f;
}

}  // namespace sockets

}  // namespace sreator

#endif //SINGLE_THREAD_REACTOR_SRC_SOCKETOPS_H_
