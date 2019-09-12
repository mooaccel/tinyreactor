//
// Created by mojiajun on 2019/9/12.
//

#include "Acceptor.h"

#include "Channel.h"
#include "SocketOps.h"

Acceptor::Acceptor(EventLoop *loop, const InetAddress listenAddr) :
    listenSocket_(monoreator::createSocket()),
    listenChannel_(loop, listenSocket_.fd()) {
    listenSocket_.bindAddress(listenAddr);
    listenChannel_.setReadCallback(&Acceptor::handleRead, this);
}

Acceptor::~Acceptor() {
    listenChannel_.disableAll();
    listenChannel_.remove();
    // listenfd在哪关闭?
}

int Acceptor::handleRead() {
    listenSocket_.accpet();
    newConnectionCallback_();
}
