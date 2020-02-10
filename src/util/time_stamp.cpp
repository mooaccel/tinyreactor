//
// Created by mojiajun on 2020/2/6.
//
#include "time_stamp.h"

#include <sys/time.h>
#include <cstdint>  // int64_t
#include <cinttypes>  // PRId64

using namespace tinyreactor;

Timestamp::Timestamp() :
    microSecondsSinceEpoch_(0) {}

Timestamp::Timestamp(int64_t microSecondSinceEpoch) :
    microSecondsSinceEpoch_(microSecondSinceEpoch) {}

Timestamp Timestamp::addTime(Timestamp timestamp, double delay_seconds) {
    // delay_seconds * 10^6, s转成us
    int64_t delta = static_cast<int64_t>(delay_seconds * kMicroSecondsPerSecond);
    return Timestamp(timestamp.microSecondsSinceEpoch() + delta);
}

std::string Timestamp::toString() const {
    char buf[32] = {0};
    int64_t seconds = microSecondsSinceEpoch_ / kMicroSecondsPerSecond;
    int64_t microseconds = microSecondsSinceEpoch_ % kMicroSecondsPerSecond;
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

Timestamp Timestamp::invalid() {
    return Timestamp{};
}