//
// Created by mojiajun on 2020/3/6.
//
#ifndef TINYREACTOR_SRC_HTTP_HTTP_REPONSE_H_
#define TINYREACTOR_SRC_HTTP_HTTP_REPONSE_H_

#include <string>
#include <map>

namespace tinyreactor {

class Buffer;

class HttpResponse {
 public:
  enum HttpStatusCode {
    kUnknown,
    k200Ok = 200,
    k301MovedPermanently = 301,
    k400BadRequest = 400,
    k404NotFound = 404,
  };

  explicit HttpResponse(bool close)
      : statusCode_(kUnknown),
        closeConnection_(close) {
  }

  void setStatusCode(HttpStatusCode code) { statusCode_ = code; }

  void setStatusMessage(const std::string &message) { statusMessage_ = message; }

  void setCloseConnection(bool on) { closeConnection_ = on; }

  bool closeConnection() const { return closeConnection_; }

  void setContentType(const std::string &contentType) { addHeader("Content-Type", contentType); }

  // FIXME: replace std::string with StringPiece
  void addHeader(const std::string &key, const std::string &value) { headers_[key] = value; }

  void setBody(const std::string &body) { body_ = body; }

  void appendToBuffer(Buffer *output) const;

 private:
  std::map<std::string, std::string> headers_;
  HttpStatusCode statusCode_;
  std::string statusMessage_;
  bool closeConnection_;
  std::string body_;
};

}
#endif //TINYREACTOR_SRC_HTTP_HTTP_REPONSE_H_
