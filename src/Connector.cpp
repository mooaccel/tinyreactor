//
// Created by 莫佳骏 on 2019/9/12.
//

#include "Connector.h"

Connector::Connector(EventLoop *loop, const InetAddress &serverAddr) : loop_(loop), serverAddr_(serverAddr) {
}
