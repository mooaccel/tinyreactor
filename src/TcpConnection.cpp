//
// Created by 莫佳骏 on 2019/9/12.
//

#include "TcpConnection.h"

#include "Channel.h"
#include "Socket.h"
#include "Channel.h"

#include <functional>
#include <iostream>
#include <memory>

#include <cstring>
#include <unistd.h>

TcpConnection::TcpConnection(EventLoop *loop,
                             std::string connname,
                             int connfd,
                             const InetAddress &localAddr,
                             const InetAddress &peerAddr) :
    loop_(loop),
    connname_(connname),
    socket_(std::make_unique<Socket>(connfd)),  // Socket's destructor will close connfd
    channel_(std::make_unique<Channel>(loop, connfd)),
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
    std::cout << "TcpConnection::ctor[" << connname_ << "] at " << this
              << " fd=" << connfd;
}

void TcpConnection::connectEstablished() {
    channel_->enableReading();
    connectionCallback_(shared_from_this()); // 建立完连接后,记录下连接的信息后,可以返回了,然后一层一层返回到epoll_wait()
}

void TcpConnection::handleRead() {
    uint8_t buf[1024];
    ::memset(buf, 0, 1024);
    ssize_t n = read(channel_->fd(), buf, 1024);
    for (int i = 0; i < 1024; ++i) {
        inputBuffer_.buffer_[i] = buf[i];
    }
    if (n > 0) {
        messageCallback_(shared_from_this(), &inputBuffer_);
    } else {
        std::cout << "TcpConnection::handleRead";
    }
}

void TcpConnection::send(const std::string &message) {
    ssize_t nwrote;
    nwrote = ::write(channel_->fd(), message.c_str(), message.size());
    if (nwrote >= 0) {
        // ...
    } else {
        fprintf(stderr, "TcpConnection::send\n");
    }
}
