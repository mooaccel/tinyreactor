//
// Created by mojiajun on 2019/9/12.
//

#include "Acceptor.h"

#include "Channel.h"
#include "InetAddress.h"
#include "SocketOps.h"

#include <functional>

using namespace monoreator::sockets;

Acceptor::Acceptor(EventLoop *loop, const InetAddress &listenAddr) :
    loop_(loop),
    listenSocket_(createSocket()),
    listenChannel_(loop, listenSocket_.fd()) {
    listenSocket_.bindAddress(listenAddr);
    listenChannel_.setReadCallback(std::bind(&Acceptor::handleRead, this));
}

Acceptor::~Acceptor() {
    listenChannel_.disableAll();
    listenChannel_.remove();
    // listenfd在哪关闭?
}

void Acceptor::handleRead() {
    InetAddress peerAddr;
    int connfd = listenSocket_.accept(&peerAddr);
    newConnectionCallback_(connfd, peerAddr);
}

void Acceptor::listen() {
    listenSocket_.listen();
    listenChannel_.enableReading();
}
