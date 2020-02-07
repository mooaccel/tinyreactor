project(tinyreactor_test)

# 找gtest
find_library(
        GTEST_LIB
        NAMES gtest libgtest libgtest_main # what to look for
        HINTS "${EXTERN_LIB_PATH}" # where to look
        NO_DEFAULT_PATH # do not search system default paths
)

# check if we found the library
message(STATUS "GTEST_LIB: [${GTEST_LIB}]")

if (NOT GTEST_LIB)
    message(SEND_ERROR "Did not find libgtest")
endif ()

# 重复了呀
include_directories(${EXTERN_INCLUDE_PATH})

set(TIMESTAMP_TEST_SOURCE_FILES time_stamp_test.cpp)
set(TIMER_TEST_SOURCE_FILES timer_test.cpp)
set(EVENTLOOP_TEST_SOURCE_FILES event_loop_test.cpp)

#add_executable(channel_test Channel_test.cpp)
#target_link_libraries(channel_test single_thread_reactor)

set(TEST_SRC_LINK
        tinyreactor)

add_executable(echoServer_test echo_server_test.cpp)
add_executable(time_stamp_test ${TIMESTAMP_TEST_SOURCE_FILES})
add_executable(timer_test ${TIMER_TEST_SOURCE_FILES})
add_executable(event_loop_test ${EVENTLOOP_TEST_SOURCE_FILES})

target_link_libraries(echoServer_test ${TEST_SRC_LINK})
target_link_libraries(time_stamp_test ${TEST_SRC_LINK})
target_link_libraries(timer_test ${TEST_SRC_LINK})
target_link_libraries(event_loop_test ${TEST_SRC_LINK})

#add_executable(echoClient_test echoClient_test.cpp)
#target_link_libraries(echoClient_test single_thread_reactor)
