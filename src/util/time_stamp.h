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

  static Timestamp now();

  static const int kMicroSecondsPerSecond = 1000 * 1000;

 private:
  /// s * 10^6 + us
  int64_t microSecondSinceEpoch_;
};

}

#endif //TINYREACTOR_SRC_UTIL_TIME_STAMP_H_
