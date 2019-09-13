//
// Created by Mo Jiajun on 2019/9/13.
//
#ifndef SINGLE_THREAD_REACTOR_SRC_BUFFER_H_
#define SINGLE_THREAD_REACTOR_SRC_BUFFER_H_

#include "string"

class Buffer {
 public:
  std::string buffer_ = std::string(1024, ' ');
};

#endif //SINGLE_THREAD_REACTOR_SRC_BUFFER_H_
