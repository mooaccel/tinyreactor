//
// Created by mojiajun on 2019/9/12.
//

#include "TcpServer.h"

#include "Acceptor.h"

#include <iostream>
#include <memory>
#include <string>
#include <functional>

using std::placeholders::_1;
using std::placeholders::_2;

TcpServer::TcpServer(EventLoop *loop, InetAddress listenAddr, std::string servername) :
    loop_(loop),
    listenIpPort_(listenAddr.toIpPort()),
    servername_(servername),
    nextConnId_(1),
    acceptor_(std::make_unique<Acceptor>(loop, listenAddr)) {
    acceptor_->setNewConnectionCallback(std::bind(TcpServer::newConnection, this, _1, _2))
}

void TcpServer::newConnection(int connfd, const InetAddress &peerAddr) {
    std::cout << "TcpServer::newConnection [" << servername_
              << "] from " << peerAddr.toIpPort();
    char buf[64];
    snprintf(buf, sizeof(buf), "-%s#%d", listenIpPort_.c_str(), nextConnId_);
    ++nextConnId_;
    std::string connName = servername_ + buf;
    InetAddress localAddr(sockets::getLocalAddr(connfd));
    TcpConnectionPtr conn(std::make_shared<TcpConnection>(loop_,  // 先暂时让它在IO Loop线程
                                                          connName,
                                                          connfd,
                                                          localAddr,
                                                          peerAddr));
    connections_[connName] = conn;
    conn->setConnectionCallback(connectionCallback_);
    conn->setMessageCallback(messageCallback_);
    conn->setCloseCallback(
        std::bind(&TcpServer::removeConnection, this, _1));
    conn->connectEstablished();
}
