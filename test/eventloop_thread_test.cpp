//
// Created by mojiajun on 2020/3/3.
//

#include "../src/event_loop.h"

#include <thread>
#include <stdio.h>
#include <unistd.h>

using namespace tinyreactor;

void print_info(EventLoop *p = nullptr) {
    printf("print: pid = %d, tid(hash) = %zu, loop = %p\n",
           getpid(), std::hash<std::thread::id>()(std::this_thread::get_id()), p);
}

int main() {
    print_info();
}

