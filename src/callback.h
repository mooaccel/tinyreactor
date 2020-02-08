//
// Created by Mo Jiajun on 2019/9/13.
//
#ifndef SINGLE_THREAD_REACTOR_SRC_CALLBACK_H_
#define SINGLE_THREAD_REACTOR_SRC_CALLBACK_H_

#include <functional>
#include <memory>

namespace tinyreactor {

class Buffer;
class TcpConnection;
using TcpConnectionPtr = std::shared_ptr<TcpConnection>;

using ConnectionCallback = std::function<void(const TcpConnectionPtr &)>;
using MessageCallback  = std::function<void(const TcpConnectionPtr &, Buffer *)>;
using TimerCallback = std::function<void()>;

}
#endif //SINGLE_THREAD_REACTOR_SRC_CALLBACK_H_
