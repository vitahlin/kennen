cmake_minimum_required(VERSION 3.19)
project(c C)
set(CMAKE_C_STANDARD 11)

# apue
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/apue/lib apue.out)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./apue/chapter10)
add_executable(catch_sigusr apue/chapter10/catch_sigusr.c)

# unp
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./other)
add_executable(get_timestamp other/get_timestamp.c)
add_executable(use_fork other/use_fork.c)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/unp/lib ${CMAKE_CURRENT_SOURCE_DIR}/unp/lib/lib.out)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./unp/chapter01)
add_executable(echo_time_tcp_srv unp/chapter01/echo_time_tcp_srv.c)
add_executable(get_time_tcp_cli unp/chapter01/get_time_tcp_cli.c)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./unp/chapter04)
add_executable(echo_time_fork_tcp_srv unp/chapter04/echo_time_fork_tcp_srv.c)
add_executable(use_connect unp/chapter04/use_connect.c)
add_executable(use_inet_pton unp/chapter04/use_inet_pton.c)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./unp/chapter05)
add_executable(tcp_srv_01 unp/chapter05/tcp_srv_01.c)
add_executable(tcp_cli_01 unp/chapter05/tcp_cli_01.c)
