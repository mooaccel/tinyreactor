//
// Created by mojiajun on 2020/2/6.
//

#include "../src/event_loop.h"

void print(const char *msg) {
    printf("msg %s\n", msg);
}

int main() {
    EventLoop loop;

    loop.runAfter(1, std::bind(print, "once1"));
    loop.runAfter(1.7, std::bind(print, "once1.7"));
    loop.runAfter(2.5, std::bind(print, "once2.5"));
    loop.runAfter(2, std::bind(print, "every2"));

    loop.loop();
}