//
// Created by 莫佳骏 on 2019/9/12.
//

#include "tcp_connection.h"

#include <unistd.h>

#include <cstring>
#include <cassert>
#include <functional>
#include <iostream>
#include <memory>

#include "channel.h"
#include "socket.h"
#include "socket_operations.h"

using namespace tinyreactor;

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
    channel_->setWriteCallback(
        std::bind(&TcpConnection::handleWrite, this));
    //channel_->setCloseCallback(
    //    std::bind(&TcpConnection::handleClose, this));
    //channel_->setErrorCallback(
    //    std::bind(&TcpConnection::handleError, this));
    std::cout << "TcpConnection::ctor[" << connname_ << "] at " << this
              << " fd=" << connfd;
}

void TcpConnection::connectEstablished() {
    // 这一行为什么不放在构造函数里?
    channel_->enableReading();
    connectionCallback_(shared_from_this()); // 建立完连接后,记录下连接的信息后,可以返回了,然后一层一层返回到epoll_wait()
    // 这个std::shared_ptr<TcpConnection>在这里结束它的生命期? TODO打个断点看看
}

void TcpConnection::handleRead() {  // connfd可读
    int savedErrno = 0;
    // 网络库这边(TcpConnection)给把数据收到Buffer
    ssize_t n = inputBuffer_.readFd(channel_->fd(), &savedErrno);
    if (n > 0) {
        messageCallback_(shared_from_this(), &inputBuffer_);
    } else if (n == 0) {
        //handleClose();
    } else {
        errno = savedErrno;
        std::cout << "TcpConnection::handleRead";
        //handleError();
    }
}

void TcpConnection::handleWrite() {  // connfd可写
    if (channel_->isMonitoringWritable()) {
        ssize_t n = sockets::write(channel_->fd(),
                                   outputBuffer_.peek(),
                                   outputBuffer_.readableBytes());
        if (n > 0) {
            outputBuffer_.retrieve(n);  // 只是改变索引
            if (outputBuffer_.readableBytes() == 0) {  // 应用缓冲区没有更多可以往socket内核缓冲区写的数据了.
                channel_->disableWriting();  // 不再监视此socket上的可写状态.否则,epoll会检测此事件,然后返回,但是其实已经没有数据可以写入内核缓冲区
            }
        } else {
            std::cerr << "TcpConnection::handleWrite" << '\n';
        }
    } else {
        std::cerr << "Connection fd = " << channel_->fd()
                  << "writable, but channel_->isMonitoringWritable() is false" << '\n';
    }
}

void TcpConnection::send(const void *data, size_t len) {
    ssize_t nwrote = 0;
    size_t remaining = len;
    // if no thing in output queue, try writing directly
    if (!channel_->isMonitoringWritable() && outputBuffer_.readableBytes() == 0) {
        nwrote = sockets::write(channel_->fd(), data, len);  // 只写一次
        std::cout << "! TcpConnection::send() write " << nwrote << " into kernel buffer" << '\n';
        if (nwrote >= 0) {
            remaining = len - nwrote;
        } else {  // nwrote < 0
            nwrote = 0;
            if (errno != EWOULDBLOCK) {
                std::cout << "TcpConnection::send()";
            }
        }
    }

    assert(remaining <= len);
    if (remaining > 0) {  // 还有剩下的没写完...(没写到socket的内核缓冲区)
        outputBuffer_.append(static_cast<const char *>(data) + nwrote, remaining);  // 存到应用缓冲区里面
        if (!channel_->isMonitoringWritable()) {
            channel_->enableWriting();  // 让epoll监视这个socket可写的状态,只要可写,就会调用TcpConnection::handleWrite()
        }
    }
}

void TcpConnection::send(const std::string &message) {
    send(message.data(), message.size());  // message.size() 不包括\0, 如"ab",返回2
}

