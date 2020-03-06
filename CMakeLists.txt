cmake_minimum_required(VERSION 2.6)

project(tinyreactor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

# CMAKE_INSTALL_PREFIX 类似./configure --prefix里的prefix
# 在本项目为了测试, 只是安装到了PROJECT_SOURCE_DIR
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(src)
add_subdirectory(test)