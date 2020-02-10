//
// Created by mojiajun on 2020/2/9.
//

#include "../src/event_loop.h"
#include "../src/util/time_stamp.h"

using namespace tinyreactor;

void print(const char *msg) {
    printf("msg %s %s\n", tinyreactor::Timestamp::now().toString().c_str(), msg);
}

int main() {
    EventLoop event_loop;

    // 2这个Timer要设置2次?timerfd_settime???对, 顺序变一下看log就能看出来, 但是Timestamp一样...why?
    event_loop.runAfterNow(3, std::bind(print, "once 3"));
    event_loop.runAfterNow(2, std::bind(print, "once 2"));

    //event_loop.runAfterNow(1.7, std::bind(print, "once 1.7"));
    //event_loop.runEvery(2, std::bind(print, "every 2"));
    //event_loop.runAt(Timestamp::addTime(Timestamp::now(), 5),
    //                 std::bind(print, "runAt once 5"));

    event_loop.loop();
}
