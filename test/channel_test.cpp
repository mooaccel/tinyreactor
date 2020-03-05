#include <functional>
//#include <map>

#include <cstdio>
#include <cstring>

#include <time.h>
#include <cinttypes>
#include <unistd.h>
#include <sys/timerfd.h>

#include "glog/logging.h"
#include "src/epoll.h"
#include "src/channel.h"
#include "src/event_loop.h"

using namespace tinyreactor;

typedef std::function<void()> TimerCallback;

void UserDefinePrint(const char *msg) {
    std::printf("UserDefinePrint() %s\n", msg);
}

// Use relative time, immunized to wall clock changes.
// 包装了一个timefd, 用来测试timefd与Channel
class PeriodicTimer {
 public:
  PeriodicTimer(EventLoop *loop, double interval, const TimerCallback &cb)
      : loop_(loop),
        timerfd_(createTimerfd()),
        timerfdChannel_(loop, timerfd_),
        interval_(interval),
        cb_(cb) {
      timerfdChannel_.setReadCallback(
          std::bind(&PeriodicTimer::handleRead, this));
      timerfdChannel_.enableReading();
  }

  void setTimer() {
      struct timespec cur_timerspec;
      struct itimerspec new_itimerspec;
      if (::clock_gettime(CLOCK_REALTIME, &cur_timerspec) == -1) {
          std::fprintf(stderr, "clock_gettime() error.\n");
      }
      new_itimerspec.it_value.tv_sec = cur_timerspec.tv_sec + 2;  // 当前时间2秒后
      new_itimerspec.it_value.tv_nsec = cur_timerspec.tv_nsec;
      new_itimerspec.it_interval.tv_sec = interval_;
      new_itimerspec.it_interval.tv_nsec = 0;
      int ret = ::timerfd_settime(timerfd_,
                                  TFD_TIMER_ABSTIME /* absolute timer */,
                                  &new_itimerspec,
                                  NULL);
      if (ret < 0) {
          std::fprintf(stderr, "timerfd_settime() error.\n");
      }
  }

  ~PeriodicTimer() {
      timerfdChannel_.disableAll();
      timerfdChannel_.remove();
      ::close(timerfd_);
  }

  void readTimerfd () {
      uint64_t howmany;
      ssize_t n = ::read(timerfd_, &howmany, sizeof howmany);
      std::printf("read Timerfd " "%" PRIu64 "\n", howmany);
      if (n != sizeof howmany)
      {
          std::fprintf(stderr, "read Timefd occur error!\n");
      }
  }

 private:
  void handleRead() {
      //readTimerfd();
      if (cb_)
          cb_();
  }

  static struct timespec toTimeSpec(double seconds) {
      struct timespec ts;
      std::memset(&ts, 0, sizeof ts);
      const int64_t kNanoSecondsPerSecond = 1000000000;
      const int kMinInterval = 100000;
      int64_t nanoseconds = static_cast<int64_t>(seconds * kNanoSecondsPerSecond);
      if (nanoseconds < kMinInterval)
          nanoseconds = kMinInterval;
      ts.tv_sec = static_cast<time_t>(nanoseconds / kNanoSecondsPerSecond);
      ts.tv_nsec = static_cast<long>(nanoseconds % kNanoSecondsPerSecond);
      return ts;
  }

  int createTimerfd() {
      int timerfd = ::timerfd_create(CLOCK_REALTIME,
                                     TFD_NONBLOCK | TFD_CLOEXEC);
      if (timerfd < 0) {
          std::fprintf(stderr, "Failed in timerfd_create\n");
      }
      return timerfd;
  }

  EventLoop *loop_;
  const int timerfd_;
  Channel timerfdChannel_;
  const double interval_; // in seconds
  TimerCallback cb_;
};

int main(int argc, char *argv[]) {
    EventLoop loop;
    PeriodicTimer periodicTimer(&loop, 1, std::bind(UserDefinePrint, "PeriodicTimer"));
    periodicTimer.setTimer();
    loop.loop();
}
