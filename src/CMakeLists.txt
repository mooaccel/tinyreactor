project(tinyreactor_lib)

set(TINYREACTOR_LIB_SRCS
        channel.cpp
        event_loop.cpp
        eventloop_thread.cpp
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

# ===========================================================
# 找glog
find_library(
        GLOG_LIB
        NAMES glog libglog # what to look for
        HINTS "${DEPS_LIB_HINT_PATH}" # where to look
        NO_DEFAULT_PATH # do not search system default paths
)

# check if we found the library
message(STATUS "In LIB, GLOG_LIB: [${GLOG_LIB}]")

if (NOT GLOG_LIB)
    message(SEND_ERROR "Did not find libglog")
endif ()

set(LIB_SRC_LINK
        ${GLOG_LIB})

add_library(tinyreactor ${TINYREACTOR_LIB_SRCS})
target_link_libraries(tinyreactor ${LIB_SRC_LINK})
