#include "../src/Epoll.h"
#include "../src/Channel.h"
#include "../src/EventLoop.h"

#include <functional>
#include <map>

#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/timerfd.h>

typedef std::function<void()> TimerCallback;

void UserDefinePrint(const char* msg)
{
  std::printf("UserDefinePrint() %s\n", msg);
}

// Use relative time, immunized to wall clock changes.
// 包装了一个timefd, 用来测试timefd与Channel
class PeriodicTimer
{
 public:
  PeriodicTimer(EventLoop* loop, double interval, const TimerCallback& cb)
    : loop_(loop),
      timerfd_(createTimerfd()),
      timerfdChannel_(loop, timerfd_),
      interval_(interval),
      cb_(cb)
  {
    timerfdChannel_.setReadCallback(
        std::bind(&PeriodicTimer::handleRead, this));
    timerfdChannel_.enableReading();
  }

  void setTimer()
  {
    struct itimerspec spec;
    std::memset(&spec, 0, sizeof spec);
    spec.it_interval = toTimeSpec(interval_);
    spec.it_value = spec.it_interval;
    int ret = ::timerfd_settime(timerfd_, 0 /* relative timer */, &spec, NULL);
    if (ret)
    {
      std::fprintf(stderr, "timerfd_settime() error.\n");
    }
  }

  ~PeriodicTimer()
  {
    //timerfdChannel_.disableAll();
    //timerfdChannel_.remove();
    ::close(timerfd_);
  }

 private:
  void handleRead()
  {
    if (cb_)
      cb_();
  }

  static struct timespec toTimeSpec(double seconds)
  {
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

  int createTimerfd()
  {
      int timerfd = ::timerfd_create(CLOCK_MONOTONIC,
                                     TFD_NONBLOCK | TFD_CLOEXEC);
      if (timerfd < 0)
      {
          std::fprintf(stderr, "Failed in timerfd_create\n");
      }
      return timerfd;
  }

  EventLoop* loop_;
  const int timerfd_;
  Channel timerfdChannel_;
  const double interval_; // in seconds
  TimerCallback cb_;
};

int main(int argc, char* argv[])
{
  EventLoop loop;
  PeriodicTimer periodicTimer(&loop, 1, std::bind(UserDefinePrint, "PeriodicTimer"));
  periodicTimer.setTimer();
  loop.loop();
}
