//
// Created by mojiajun on 2019/9/12.
//

#include "Socket.h"

#include "InetAddress.h"
#include "SocketOps.h"

#include <cstring>

void Socket::accept(InetAddress *peeraddr) {  // 成功之后把对端的信息填入到peeraddr
    struct sockaddr_in cliaddr;
    socklen_t cliaddrlen = static_cast<socklen_t>(sizeof cliaddr);

    int connfd = ::accept(socket_fd_, sockaddr_cast(&cliaddr), &cliaddrlen);
    // accept4()和accept()的优缺点

    if (connfd < 0)  // accept的错误处理
    {
        int savedErrno = errno;
        fprintf(stderr, "Socket::accept() error\n");
        switch (savedErrno)
        {
            case EAGAIN:
            case ECONNABORTED:
            case EINTR:
            case EPROTO: // ???
            case EPERM:
            case EMFILE: // per-process lmit of open file desctiptor ???
                // expected errors
                errno = savedErrno;
                break;
            case EBADF:
            case EFAULT:
            case EINVAL:
            case ENFILE:
            case ENOBUFS:
            case ENOMEM:
            case ENOTSOCK:
            case EOPNOTSUPP:
                // unexpected errors
                fprintf(stderr, "unexpected error of ::accept, errno: %d\n", savedErrno);
                break;
            default:
                fprintf(stderr, "unexpected error of ::accept, errno: %d\n", savedErrno);
                break;
        }
    }
    peeraddr->setSockAddr(cliaddr);
}

void Socket::bindAddress(const InetAddress &listenaddr) {
    int ret = ::bind(socket_fd_, listenaddr.getSockAddr(), static_cast<socklen_t>(sizeof(struct sockaddr_in)));
    if (ret < 0)
    {
        fprintf(stderr, "Socket::bindAddress\n");
    }
}
