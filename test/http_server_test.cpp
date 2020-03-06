//
// Created by mojiajun on 2020/3/6.
//
#include <iostream>
#include <map>

#include "glog/logging.h"
#include "src/event_loop.h"
#include "src/http/http_server.h"
#include "src/inet_address.h"

using namespace tinyreactor;

/// 填充业务代码
/// 请求在HttpRequest里面, 作为输入
/// 回应在HttpResponse里面, 作为输出
void onRequest(const HttpRequest &req, HttpResponse *resp) {
    std::cout << "Headers " << req.methodString() << " " << req.path() << std::endl;

    if (req.path() == "/") {
        resp->setStatusCode(HttpResponse::k200Ok);
        resp->setStatusMessage("OK");
        resp->setContentType("text/html");
        resp->addHeader("Server", "httpserver");
        std::string now = Timestamp::now().toFormattedString();
        resp->setBody("<html><head><title>This is title</title></head>"
                      "<body><h1>Hello</h1>Now is " + now +
            "</body></html>");
    } else if (req.path() == "/hello") {
        resp->setStatusCode(HttpResponse::k200Ok);
        resp->setStatusMessage("OK");
        resp->setContentType("text/plain");
        resp->addHeader("Server", "Muduo");
        resp->setBody("hello, world!\n");
    } else {
        resp->setStatusCode(HttpResponse::k404NotFound);
        resp->setStatusMessage("Not Found");
        resp->setCloseConnection(true);
    }
}

int main(int argc, char **argv) {
    // 不输出至logfile, 输出到stderr
    FLAGS_logtostderr = 1;
    // >=0级的log都输出出来
    FLAGS_minloglevel = 0;

    ::google::InitGoogleLogging(argv[0]);
    LOG(INFO) << "http server launch.";

    EventLoop loop;
    // 在云服务器上使用curl访问, 或者直接用一个浏览器打开http://阿里云的外网ip:8000/
    // 要在网页控制端打开防火墙
    // 设置为阿里云的内网ip, InetAddress("阿里云的内网ip", 8000)
    // 参考https://www.cnblogs.com/wuzaipei/p/10729326.html
    HttpServer server(&loop, InetAddress("127.0.0.1", 8000), "http_test_server");
    server.setHttpCallback(onRequest);
    server.startListen();
    loop.loop();
}
