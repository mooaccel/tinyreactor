project(tinyreactor_lib)

set(TINYREACTOR_LIB_SRCS
        channel.cpp
        event_loop.cpp
        epoll.cpp
        acceptor.cpp
        socket.cpp
        tcp_server.cpp
        socket_operations.cpp
        tcp_client.cpp
        connector.cpp
        tcp_connection.cpp
        inet_address.cpp
        buffer.cpp
        ./util/time_stamp.cpp
        ./util/timer.cpp
        ./util/timer_queue.cpp
        )

add_library(tinyreactor ${TINYREACTOR_LIB_SRCS})
