cmake_minimum_required(VERSION 3.5)

project(waystation)

add_executable(waystation main.cpp tcp_link.cpp tunnel_frame.cpp)
set_target_properties(waystation PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
target_compile_options(waystation PUBLIC -Wall -Wextra -Wpedantic)
