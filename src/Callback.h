//
// Created by Mo Jiajun on 2019/9/13.
//
#ifndef SINGLE_THREAD_REACTOR_SRC_CALLBACK_H_
#define SINGLE_THREAD_REACTOR_SRC_CALLBACK_H_

#include <functional>

using ConnectionCallback = std::function<void(const TcpConnectionPtr &)>;
using MessageCallback  = std::function<void(const TcpConnectionPtr &, Buffer *)>;

#endif //SINGLE_THREAD_REACTOR_SRC_CALLBACK_H_
