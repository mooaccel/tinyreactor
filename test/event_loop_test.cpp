//
// Created by mojiajun on 2020/2/7.
//

#include "../src/event_loop.h"
#include <iostream>
#include <thread>

using namespace tinyreactor;

void _other_thread_test(EventLoop *loop) {
    std::cout << " sub thread " << loop->isInLoopThread() << '\n';
}

int main() {
    EventLoop loop;
    loop.isInLoopThread();
    std::thread th1(_other_thread_test, &loop);
    th1.join();
    std::cout << " loop thread " << loop.isInLoopThread() << '\n';
}
