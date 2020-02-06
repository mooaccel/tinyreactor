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

set(TIMESTAMP_TEST_SOURCE_FILES time_stamp_test.cpp)

#add_executable(channel_test Channel_test.cpp)
#target_link_libraries(channel_test single_thread_reactor)

add_executable(echoServer_test echo_server_test.cpp)
add_executable(time_stamp_test ${TIMESTAMP_TEST_SOURCE_FILES})

target_link_libraries(echoServer_test tinyreactor)
target_link_libraries(time_stamp_test tinyreactor)

#add_executable(echoClient_test echoClient_test.cpp)
#target_link_libraries(echoClient_test single_thread_reactor)
