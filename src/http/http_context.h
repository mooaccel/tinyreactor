//
// Created by mojiajun on 2020/3/6.
//
#ifndef TINYREACTOR_SRC_HTTP_HTTP_CONTEXT_H_
#define TINYREACTOR_SRC_HTTP_HTTP_CONTEXT_H_

#include "src/buffer.h"
#include "http_request.h"

namespace tinyreactor {

class HttpContext {
 public:
  /// 目前解析的状态
  enum HttpRequestParseState {
    kExpectRequestLine,
    kExpectHeaders,
    kExpectBody,
    kGotAll,
  };

  HttpContext()
      : http_req_parse_state_(kExpectRequestLine) {
  }

  bool parseRequestFromBuffer(Buffer *buf);

  /// 已经获取了一个完整的http请求
  bool gotAll() const { return http_req_parse_state_ == kGotAll; }

  /// 重置http_req_parse_state_, 回到初始状态
  /// 重置HttpRequest
  void reset() {
      http_req_parse_state_ = kExpectRequestLine;
      HttpRequest dummy;
      request_.swap(dummy);
  }

  /// 返回HttpContext里保存的HttpRequest的const引用
  const HttpRequest &request() const { return request_; }

  /// 返回HttpContext里保存的HttpRequest的不可变引用
  HttpRequest &request() { return request_; }

 private:
  bool processRequestLine(const char *begin, const char *end);

  HttpRequestParseState http_req_parse_state_;
  HttpRequest request_;
};

}

#endif //TINYREACTOR_SRC_HTTP_HTTP_CONTEXT_H_
