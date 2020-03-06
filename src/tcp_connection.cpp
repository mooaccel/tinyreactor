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

#include "glog/logging.h"
#include "channel.h"
#include "event_loop.h"
#include "socket.h"
#include "socket_operations.h"

using namespace tinyreactor;

TcpConnection::TcpConnection(EventLoop *loop,
                             std::string connname,
                             int connfd,
                             const InetAddress &localAddr,
                             const InetAddress &peerAddr) :
    ioloop_(loop),
    connname_(connname),
    state_(kConnecting),  // 初始化TcpConnection的时候kConnecting, 内核里的tcp连接可能已经建立好了
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

// 在TcpServer::newConnection()的最后调用它
void TcpConnection::connectEstablished() {
    ioloop_->assertInLoopThread();
    // 之前的状态必须是kConnecting
    assert(state_ == kConnecting);
    // 设置TcpConnection里的state_为已连接状态
    setState(kConnected);
    // 这一行为什么不放在构造函数里?
    channel_->enableReading();
    connectionCallback_(shared_from_this()); // 建立完连接后,记录下连接的信息后,可以返回了,然后一层一层返回到epoll_wait()
    // 这个std::shared_ptr<TcpConnection>在这里结束它的生命期? TODO打个断点看看
}

void TcpConnection::handleRead() {  // connfd可读
    int savedErrno = 0;
    // 网络库这边(TcpConnection)给把数据从内核缓冲区收到Buffer里面
    ssize_t n = inputBuffer_.readFd(channel_->fd(), &savedErrno);
    // 一次没读完怎么办?
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

/// Reactor发现这个connfd可读的时候调用handleWrite()
void TcpConnection::handleWrite() {  // connfd可写
    // 确保handleWrite是在io线程执行的
    ioloop_->assertInLoopThread();
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

// TODO, data的数据在哪? 谁负责释放?
void TcpConnection::send(const void *data, size_t len) {
    send(std::string(static_cast<const char *>(data), len));
}

void TcpConnection::send(const std::string &message) {
    if (state_ != kConnected)
        return;
    // 如果是在ioloop调用的send
    // 那么直接调用sendInLoop发送这部分数据
    if (ioloop_->isInLoopThread()) {
        sendInLoop(message);
    } else { // 到io线程调用sendInLoop
        // TODO, 待考量
        //ioloop_->runInLoop(
        //    std::bind(&TcpConnection::sendInLoop,
        //              message));
        //??
        void (TcpConnection::*fp)(const std::string& message) = &TcpConnection::sendInLoop;
        ioloop_->runInLoop(
            std::bind(fp,
                      this,     // FIXME
                      message));
        //std::forward<string>(message)));
    }
}

/// 待发送的数据的第一个字节是data指向的位置
/// 有len个字节数据待发送
void TcpConnection::sendInLoop(const void *data, size_t len) {
    ioloop_->assertInLoopThread();
    // assert(len > 0); 需要吗?
    ssize_t nwrote = 0;
    size_t remaining = len;
    bool write_error = false;

    // 如果准备发送的时候, 连接已经断开了.
    if (state_ == kDisconnected) {
        LOG(INFO) << "disconnected, give up writing";
        return;
    }
    // 如果Channel没有关注可写事件, 而且TcpConnection里的outputBuffer_需要写入内核缓冲区的数据为0
    // 对于outputbuffer, readable代表这部分数据还没有写入内核发送缓冲区.
    // writeable代表outputbuffer的使用者还可以往outputbuffer塞数据吗
    // 对于inputbuffer, readable代表有从内核缓冲区来的数据还在inputbuffer里
    // inputbuffer的writeable代表内核缓冲区的数据能往inputbuffer里塞数据吗

    if (!channel_->isMonitoringWritable() && outputBuffer_.readableBytes() == 0) {
        // 这一次直接写, 不用等下一次
        // 能写多少是多少
        nwrote = sockets::write(channel_->fd(), data, len);
        if (nwrote >= 0) {
            remaining = len - nwrote;
            if (remaining == 0) {
                // TODO
            }
        } else {  // nwrote < 0
            nwrote = 0;
            // TODO, 错误处理待分析
            if (errno != EWOULDBLOCK) {
                LOG(ERROR) << "TcpConnection::sendInLoop() write data to kernel tcp buffer error";
                if (errno == EPIPE || errno == ECONNRESET) // FIXME: any others?
                {
                    write_error = true;
                }
            }
        }
    }

    assert(remaining <= len);
    // 如果之前没有发生写入错误, 而且还有剩下的数据没有塞入内核发送缓冲区
    if (!write_error && remaining > 0) {
        // 把剩下的数据添加到outputBuffer_里面
        outputBuffer_.append(static_cast<const char *>(data) + nwrote, remaining);
        // 然后设置这个TcpConnection所在的Reactor io线程针对TcpConnection对应的Channel可写状态感兴趣
        if (!channel_->isMonitoringWritable()) {
            channel_->enableWriting();
            // 让epoll监视这个socket可写的状态,只要可写,就会调用TcpConnection::handleWrite()
        }
    }
}

void TcpConnection::sendInLoop(const std::string &message) {
    sendInLoop(message.data(), message.size());
}
