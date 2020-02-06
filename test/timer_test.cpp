//
// Created by mojiajun on 2020/2/6.
//

#include "../src/util/time_stamp.h"
#include "../src/event_loop.h"
#include "../src/channel.h"

#include <sys/timerfd.h>
#include <cstring>

void print(const char *msg) {
    printf("msg %s %s\n", tinyreactor::Timestamp::now().toString().c_str(), msg);
}

void timeout() {
    printf("Timeout!\n");
}

int main() {
    EventLoop loop;

    //loop.runAfter(1, std::bind(print, "once1"));
    //loop.runAfter(1.7, std::bind(print, "once1.7"));
    //loop.runAfter(2.5, std::bind(print, "once2.5"));
    //loop.runEvery(2, std::bind(print, "every2"));

    // 这几个flag
    int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    Channel channel(&loop, timerfd);
    channel.setReadCallback(timeout);
    channel.enableReading();

    struct itimerspec howlong;
    ::memset(&howlong, 0, sizeof howlong);
    howlong.it_value.tv_sec = 5;
    // 5s之后expire
    ::timerfd_settime(timerfd, 0, &howlong, nullptr);

    loop.loop();
}