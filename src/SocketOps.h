//
// Created by 莫佳骏 on 2019/9/12.
//

#ifndef SINGLE_THREAD_REACTOR_SRC_SOCKETOPS_H_
#define SINGLE_THREAD_REACTOR_SRC_SOCKETOPS_H_

#include <cstdint>

namespace monoreator {

namespace sockets {

int createSocket();

void generateAddrfromIpPort(const char* ip, uint16_t port, struct sockaddr_in *addr);
const struct sockaddr *sockaddr_cast(const struct sockaddr_in *addr);
const struct sockaddr_in *sockaddr_in_cast(const struct sockaddr *addr);

}  // namespace sockets

}  // namespace sreator

#endif //SINGLE_THREAD_REACTOR_SRC_SOCKETOPS_H_
