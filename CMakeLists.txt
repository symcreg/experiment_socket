cmake_minimum_required(VERSION 3.27)
project(net_socket)

set(CMAKE_CXX_STANDARD 20)


add_executable(net_socket main.cpp
        server.cpp
        server.h)

target_link_libraries(net_socket ws2_32)