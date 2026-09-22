cmake_minimum_required(VERSION 3.5)

project(waystation)

add_executable(waystation main.cpp tcp_link.cpp tunnel_frame.cpp identity_codec.cpp padding_codec.cpp)
set_target_properties(waystation PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
target_compile_options(waystation PUBLIC -Wall -Wextra -Wpedantic)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/googletest" "googletest")
add_executable(waystation_tests tests/frame_tests.cpp tunnel_frame.cpp tests/identity_tests.cpp identity_codec.cpp
                tests/padding_tests.cpp padding_codec.cpp)
set_target_properties(waystation_tests PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
target_compile_options(waystation_tests PUBLIC -Wall -Wextra -pedantic)
target_link_libraries(waystation_tests PUBLIC gtest gtest_main)
