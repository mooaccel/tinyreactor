//
// Created by mojiajun on 2020/2/6.
//
#ifndef TINYREACTOR_SRC_UTIL_TIME_STAMP_H_
#define TINYREACTOR_SRC_UTIL_TIME_STAMP_H_

#include <cstdint>
#include <string>

namespace tinyreactor {

class Timestamp {
 public:
  Timestamp();
  explicit Timestamp(int64_t microSecondSinceEpoch);
  ~Timestamp() = default;

  std::string toString() const;
  std::string toFormattedString(bool showMicroseconds = true) const;

  int64_t microSecondsSinceEpoch() const {
      return microSecondsSinceEpoch_;
  }

  bool valid() const {
      return microSecondsSinceEpoch_ > 0;
  }

  /// 返回调用now()时的Timestamp
  static Timestamp now();

  static Timestamp invalid();

  /// 从timestamp开始加delay_seconds
  static Timestamp addTime(Timestamp timestamp, double delay_seconds);

  static const int kMicroSecondsPerSecond = 1000 * 1000;

 private:
  /// s * 10^6 + us
  int64_t microSecondsSinceEpoch_;
};

inline bool operator<(Timestamp lhs, Timestamp rhs) {
    return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}

inline bool operator<=(Timestamp lhs, Timestamp rhs) {
    return lhs.microSecondsSinceEpoch() <= rhs.microSecondsSinceEpoch();
}

inline bool operator==(Timestamp lhs, Timestamp rhs) {
    return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
}

}

#endif //TINYREACTOR_SRC_UTIL_TIME_STAMP_H_
