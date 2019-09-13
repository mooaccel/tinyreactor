//
// Created by mojiajun on 2019/9/12.
//

#ifndef SRC_SOCKET_H_
#define SRC_SOCKET_H_

class InetAddress;

class Socket {
 public:
  explicit Socket(int socket_fd) : socket_fd_(socket_fd) {
  }
  ~Socket();
  void bindAddress(const InetAddress &listenaddr);
  int accept(InetAddress *peeraddr);
  int fd() {
    return socket_fd_;
  }

  void listen();

 private:
  int socket_fd_;
};

#endif //SRC_SOCKET_H_
