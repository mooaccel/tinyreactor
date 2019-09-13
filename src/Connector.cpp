//
// Created by 莫佳骏 on 2019/9/12.
//

#include "Connector.h"

#include "EventLoop.h"
#include "InetAddress.h"

Connector::Connector(EventLoop *loop, const InetAddress &serverAddr) : loop_(loop), serverAddr_(serverAddr) {
}
