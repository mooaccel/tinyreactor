//
// Created by mojiajun on 2019/9/12.
//

#include "tcp_server.h"

#include "acceptor.h"
#include "inet_address.h"
#include "socket_operations.h"
#include "tcp_connection.h"

#include <iostream>
#include <memory>
#include <string>
#include <functional>

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
    std::cout << "TcpServer::newConnection [" << servername_
              << "] from " << peerAddr.toIpPort();
    char buf[64];
    snprintf(buf, sizeof(buf), "-%s#%d", listenIpPort_.c_str(), nextConnId_);
    ++nextConnId_;
    std::string connName = servername_ + buf;
    InetAddress localAddr(tinyreactor::sockets::getLocalAddr(connfd));
    TcpConnectionPtr conn(std::make_shared<TcpConnection>(loop_,  // 先暂时让它在IO Loop线程
                                                          connName,
                                                          connfd,
                                                          localAddr,
                                                          peerAddr));
    connections_[connName] = conn;
    // 把TcpServer里面存的cb传给TcpConnection
    conn->setConnectionCallback(connectionCallback_);
    conn->setMessageCallback(messageCallback_);
    conn->connectEstablished();
}
