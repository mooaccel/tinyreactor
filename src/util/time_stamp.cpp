//
// Created by mojiajun on 2020/2/6.
//
#include "time_stamp.h"

#include <sys/time.h>
#include <cinttypes>  // PRId64

using namespace tinyreactor;

Timestamp::Timestamp() {}

Timestamp::Timestamp(int64_t microSecondSinceEpoch) :
    microSecondSinceEpoch_(microSecondSinceEpoch) {}

std::string Timestamp::toString() const {
    char buf[32] = {0};
    int64_t seconds = microSecondSinceEpoch_ / kMicroSecondsPerSecond;
    int64_t microseconds = microSecondSinceEpoch_ % kMicroSecondsPerSecond;
    snprintf(buf, sizeof(buf) - 1, "%" PRId64 ".%06" PRId64 "",
             seconds,
             microseconds);
    return buf;
}

Timestamp Timestamp::now() {
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    int64_t seconds = tv.tv_sec;
    return Timestamp(seconds * kMicroSecondsPerSecond + tv.tv_usec);
}