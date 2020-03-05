//
// Created by mojiajun on 2019/9/12.
//

#include "tcp_server.h"

#include <iostream>
#include <memory>
#include <string>
#include <functional>

#include "acceptor.h"
#include "event_loop.h"
#include "inet_address.h"
#include "socket_operations.h"
#include "tcp_connection.h"

using std::placeholders::_1;
using std::placeholders::_2;

using namespace tinyreactor;

TcpServer::TcpServer(EventLoop *loop, const InetAddress &listenAddr, std::string servername) :
    loop_(loop),
    listenIpPort_(listenAddr.toIpPort()),
    servername_(servername),
    nextConnId_(1),
    acceptor_(std::make_unique<Acceptor>(loop, listenAddr)) {
    // 给Acceptor的readable设置回调函数
    acceptor_->setNewConnectionCallback(std::bind(&TcpServer::newConnection, this, _1, _2));  // ?为什么要加上&
}

/// @connfd Acceptor accpet()之后返回的连接套接字
/// @peerAddr 客户端的地址
void TcpServer::newConnection(int connfd, const InetAddress &peerAddr) {
    // 确保在监听fd所在的loop中执行TcpServer::newConnection()
    loop_->assertInLoopThread();
    // FIXME: 用glog? 要确保线程安全
    std::cout << "TcpServer::newConnection [" << servername_
              << "] from " << peerAddr.toIpPort();
    char buf[64];
    snprintf(buf, sizeof(buf), "-%s#%d", listenIpPort_.c_str(), nextConnId_);
    ++nextConnId_;
    std::string connName = servername_ + buf;
    InetAddress localAddr(tinyreactor::sockets::getLocalAddr(connfd));
    // 先暂时让conn的read和write发生在IO Loop线程
    EventLoop *ioLoop = loop_;
    TcpConnectionPtr conn(std::make_shared<TcpConnection>(ioLoop,
                                                          connName,
                                                          connfd,
                                                          localAddr,
                                                          peerAddr));
    connections_[connName] = conn;
    // 把TcpServer里面存的cb传给TcpConnection
    conn->setConnectionCallback(connectionCallback_);
    conn->setMessageCallback(messageCallback_);
    // TcpConnection::connectEstablished需要TcpConnection示例的指针
    // 这里传入conn, 其能转成raw pointer
    ioLoop->runInLoop(std::bind(&TcpConnection::connectEstablished, conn));
}
