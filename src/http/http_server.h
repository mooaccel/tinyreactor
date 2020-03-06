//
// Created by mojiajun on 2020/3/6.
//
#ifndef TINYREACTOR_SRC_HTTP_SERVER_H_
#define TINYREACTOR_SRC_HTTP_SERVER_H_

#include <string>

#include "src/buffer.h"
#include "src/inet_address.h"
#include "src/tcp_server.h"
#include "http_request.h"
#include "http_response.h"

namespace tinyreactor {

class HttpServer {
 public:
  using HttpCallback = std::function<void(const HttpRequest &, HttpResponse *)>;

  HttpServer(EventLoop *loop,
             const InetAddress &listenAddr,
             const std::string &name);

  ~HttpServer() = default;

  void setHttpCallback(const HttpCallback &cb) {
      http_cb_ = cb;
  }

  /// httpserver开始监听
  void startListen();

 private:
  /// 收到连接后需要执行的回调
  void onConnection(const TcpConnectionPtr &conn);

  /// tcp收到信息后, 回调这个函数, 不一定是完整消息, 所以需要处理不是完整消息的情况
  void onMessage(const TcpConnectionPtr &conn, Buffer *buf);

  /// onMessage解析的是字节流, onRequest解析的是完整的消息
  void onRequest(const TcpConnectionPtr &conn, const HttpRequest &req);

  TcpServer server_;

  /// 给用户提供的接口, 用来填充业务代码
  /// 请求在HttpRequest里面, 作为输入
  /// 回应在HttpResponse里面, 作为输出
  HttpCallback http_cb_;
};

}
#endif //TINYREACTOR_SRC_HTTP_SERVER_H_
