//
// Created by mojiajun on 2019/9/9.
//

#ifndef SRC_CHANNEL_H_
#define SRC_CHANNEL_H_

#include <functional>

class EventLoop;

// Channel 负责事件分发
// 每个Channel只负责一个fd
class Channel {
 public:
  using EventCallback = std::function<void()>;

  Channel(EventLoop *loop, int fd) : loopOwner_(loop), fd_(fd) {}
  ~Channel() = default;

  Channel(const Channel &) = delete;
  Channel &operator=(const Channel &) = delete;

  void handleEvent();

  int events() const { return events_; }
  void set_events(int events) { events_ = events; }
  int revents() const { return revents_; }
  void set_revents(int revents) { revents_ = revents; }
  void set_fd(int fd) { fd_ = fd; }
  int fd() const { return fd_; }

  bool isMonitoringWritable() { return events_ & kWriteEvent; }
  bool isMonitoringReadable() { return events_ & kReadEvent; }

  void remove();  // 这个API设计的...感觉并不好

  void enableReading() {
      events_ |= kReadEvent;
      update();
  }
  void disableReading() {
      events_ &= ~kReadEvent;
      update();
  }
  void enableWriting() {
      events_ |= kWriteEvent;
      update();
  }
  void disableWriting() {
      events_ &= ~kWriteEvent;
      update();
  }
  void disableAll() {
      events_ = kNoneEvent;
      update();
  }
  bool isWriting() const { return events_ & kWriteEvent; }
  bool isReading() const { return events_ & kReadEvent; }

  void setReadCallback(EventCallback cb) { readCallback_ = std::move(cb); }
  void setWriteCallback(EventCallback cb) { writeCallback_ = std::move(cb); }

 private:
  static const int kNoneEvent;
  static const int kReadEvent;
  static const int kWriteEvent;

  void update();

  EventLoop *loopOwner_;
  int fd_;
  int events_;
  int revents_;

  EventCallback readCallback_;
  EventCallback writeCallback_;
};

#endif  // SRC_CHANNEL_H_
