//
// Created by 莫佳骏 on 2019/9/12.
//

#include "TcpClient.h"

TcpClient::TcpClient(EventLoop *loop, const InetAddress &serverAddr, const std::string &name) :
    loop_(loop),
    connector_(loop, serverAddr),
    name_(name) {

}

TcpClient::~TcpClient() {

}
