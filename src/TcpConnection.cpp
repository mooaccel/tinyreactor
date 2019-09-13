//
// Created by 莫佳骏 on 2019/9/12.
//

#include "TcpConnection.h"

#include <iostream>
#include <memory>

TcpConnection::TcpConnection(EventLoop *loop,
                             std::string connname,
                             int connfd,
                             const InetAddress &localAddr,
                             const InetAddress &peerAddr) :
    loop_(loop),
    connname_(connname),
    socket_(std::make_unique<Socket>(connfd)),  // Socket's destructor will close connfd
    channel_(std::make_unique<Channel>(connfd, loop)),
    localAddr_(localAddr),
    peerAddr_(peerAddr) {

    channel_->setReadCallback(
        std::bind(&TcpConnection::handleRead, this));
    //channel_->setWriteCallback(
    //    std::bind(&TcpConnection::handleWrite, this));
    //channel_->setCloseCallback(
    //    std::bind(&TcpConnection::handleClose, this));
    //channel_->setErrorCallback(
    //    std::bind(&TcpConnection::handleError, this));
    std::cout << "TcpConnection::ctor[" << name_ << "] at " << this
              << " fd=" << connfd;
}

void TcpConnection::connectEstablished() {
    channel_->enableReading();
    connectionCallback_(shared_from_this()); // 建立完连接后,记录下连接的信息后,可以返回了,然后一层一层返回到epoll_wait()
}

void TcpConnection::handleRead()
{
    int savedErrno = 0;
    ssize_t n = inputBuffer_.readFd(channel_->fd(), &savedErrno);
    if (n > 0)
    {
        messageCallback_(shared_from_this(), &inputBuffer_);
    }
    else if (n == 0)
    {
        std::cout << "TcpConnection::handleRead()";
    }
    else
    {
        errno = savedErrno;
        std::cout << "TcpConnection::handleRead";
    }
}
