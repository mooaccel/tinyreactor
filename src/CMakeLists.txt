set(lib_SRCS
        Channel.cpp
        EventLoop.cpp
        Epoll.cpp
        Acceptor.cpp
        Socket.cpp
        TcpServer.cpp
        SocketOps.cpp
        TcpClient.cpp
        Connector.cpp)

add_library(single_thread_reactor ${lib_SRCS})

set(HEADERS
        Channel.h
        EventLoop.h
        Epoll.h
        Acceptor.h
        Socket.h
        TcpServer.h
        SocketOps.h
        TcpClient.h
        Connector.h
        )
