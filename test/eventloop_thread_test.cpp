//
// Created by mojiajun on 2020/3/3.
//

#include <chrono>
#include <thread>
#include <cstdio>
#include <unistd.h>

#include "../src/event_loop.h"
#include "../src/eventloop_thread.h"

using namespace tinyreactor;

void print_info(EventLoop *p = nullptr) {
    std::printf("print: pid = %d, tid(hash) = %zu, loop = %p\n",
                getpid(), std::hash<std::thread::id>()(std::this_thread::get_id()), p);
}

int main() {
    print_info();
    //std::thread th1(print_info, nullptr);
    //th1.join();
    {
        // dtor calls quit()
        EventLoopThread thr1;
        EventLoop *loop = thr1.getEventLoop();
        // 能拿到loop, 但是没法让它执行runInLoop
        loop->runInLoop(std::bind(print_info, loop));
        printf("execute point \n");
    }
}

