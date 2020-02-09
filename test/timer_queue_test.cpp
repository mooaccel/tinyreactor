//
// Created by mojiajun on 2020/2/9.
//

#include "../src/event_loop.h"

using namespace tinyreactor;

void print(const char *msg) {
    printf("msg %s %s\n", tinyreactor::Timestamp::now().toString().c_str(), msg);
}

int main() {
    EventLoop event_loop;
    event_loop.runAfterNow(2, std::bind(print, "once2"));
    //event_loop.runAfterNow(1.7, std::bind(print, "once1.7"));
    //event_loop.runAfterNow(2.5, std::bind(print, "once2.5"));
    //event_loop.runEvery(2, std::bind(print, "every2"));
    event_loop.loop();
}

