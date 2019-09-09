set(lib_SRCS
        Channel.cpp
        EventLoop.cpp
        Epoll.cpp
        )

add_library(single_thread_reactor ${lib_SRCS})

set(HEADERS
        Channel.h
        EventLoop.h
        Epoll.h
        )
