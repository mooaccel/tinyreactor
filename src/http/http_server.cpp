//
// Created by mojiajun on 2020/3/6.
//
#include "src/http/http_server.h"

#include <string>

#include "glog/logging.h"
#include "src/tcp_connection.h"
#include "http_context.h"

using namespace tinyreactor;
using namespace std::placeholders;

HttpServer::HttpServer(EventLoop *loop,
                       const InetAddress &listen_addr,
                       const std::string &server_name)
    : server_(loop, listen_addr, server_name) {
    server_.setConnectionCallback(
        std::bind(&HttpServer::onConnection, this, _1));
    server_.setMessageCallback(
        std::bind(&HttpServer::onMessage, this, _1, _2));
}

void HttpServer::startListen() {
    LOG(INFO) << "HttpServer " << server_.tcp_server_name()
              << "start listening on " << server_.ipPort();
    server_.startListen();
}

void HttpServer::onConnection(const TcpConnectionPtr &conn) {
    LOG(INFO) << "HttpServer::onConnection() callback";
    // 正常情况下, TcpConnection::connectEstablished()最后回调它的时候, 状态已经是kConnected了.
    if (!conn->isConnected()) {
        LOG(ERROR) << "TcpConnection not in kConnected状态";
    }
    conn->setContext(HttpContext());
}

void HttpServer::onMessage(const TcpConnectionPtr &conn,
                           Buffer *buf) {
    LOG(INFO) << "HttpServer::onMessage() callback";
    LOG(INFO) << "http server receive byte datas is :\n" << buf->retrieveAllAsStringButNotModiftBufferState();
    // 有消息到来, 得到目前的上下文
    // HttpContext *ctx类型
    auto ctx = std::any_cast<HttpContext>(conn->getMutableContext());

    // 处理buf里的字节数据流
    if (!ctx->parseRequestFromBuffer(buf)) {
        conn->send("HTTP/1.1 400 Bad Request\r\n\r\n");
        //conn->shutdown();
    }

    // 如果HttpContext已经获取了整个消息帧
    if (ctx->gotAll()) {
        // 处理这个请求, 从ctx中拿到HttpRequest的引用
        onRequest(conn, ctx->request());
        // ctx里头的那个请求已经被处理完了, 需要重置它
        ctx->reset();
    }
}

void HttpServer::onRequest(const TcpConnectionPtr &conn,
                           const HttpRequest &req) {
    const std::string &connection = req.getHeader("Connection");
    bool close = connection == "close" ||
        (req.getVersion() == HttpRequest::kHttp10 && connection != "Keep-Alive");
    HttpResponse response(close);
    // 回调用户提供的业务代码
    http_cb_(req, &response);
    Buffer buf;
    // 把response对象序列化到buf里面
    response.appendToBuffer(&buf);
    // 然后用conn发送这个buf里的所有readable范围内的字节
    conn->send(&buf);
    // 如果用户设置了关闭连接
    if (response.closeConnection()) {
        // TODO, 先不考虑关闭
        //conn->shutdown();
    }
}
