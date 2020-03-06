//
// Created by mojiajun on 2020/3/6.
//
#ifndef TINYREACTOR_SRC_HTTP_HTTP_REQUEST_H_
#define TINYREACTOR_SRC_HTTP_HTTP_REQUEST_H_

#include <map>
#include <string>
#include <cassert>

namespace tinyreactor {

class HttpRequest {
 public:
  /// http请求Method
  enum Method {
    kInvalid, kGet, kPost, kHead, kPut, kDelete
  };
  /// http版本号
  enum Version {
    kUnknown, kHttp10, kHttp11
  };

  HttpRequest()
      : method_(kInvalid),
        version_(kUnknown) {
  }

  void setVersion(Version ver) {
      version_ = ver;
  }

  Version getVersion() const { return version_; }

  bool setMethod(const char *start, const char *end);

  Method method() const { return method_; }

  const char *methodString() const;

  void setPath(const char *start, const char *end) {
      path_.assign(start, end);
  }

  const std::string &path() const { return path_; }

  void setQuery(const char *start, const char *end) {
      query_.assign(start, end);
  }

  const std::string &query() const { return query_; }

  void addHeader(const char *start, const char *colon, const char *end);

  std::string getHeader(const std::string &field) const;

  const std::map<std::string, std::string> &headers() const { return headers_; }

  void swap(HttpRequest &that) {
      std::swap(method_, that.method_);
      std::swap(version_, that.version_);
      path_.swap(that.path_);
      query_.swap(that.query_);
      headers_.swap(that.headers_);
  }

 private:
  /// http请求method
  Method method_;
  /// 版本号
  Version version_;

  std::string path_;
  std::string query_;
  std::map<std::string, std::string> headers_;
};

}
#endif //TINYREACTOR_SRC_HTTP_HTTP_REQUEST_H_
