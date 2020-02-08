//
// Created by 莫佳骏 on 2019/9/12.
//

#include "connector.h"

#include "event_loop.h"
#include "inet_address.h"

using namespace tinyreactor;

Connector::Connector(EventLoop *loop, const InetAddress &serverAddr) : loop_(loop), serverAddr_(serverAddr) {}
