cmake_minimum_required(VERSION 3.24)
project(server)
project(client)

set(CMAKE_CXX_STANDARD 98)

add_executable(client client_main.cpp)
add_executable(server server_main.cpp)
