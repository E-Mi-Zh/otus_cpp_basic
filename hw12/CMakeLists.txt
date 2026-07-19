cmake_minimum_required(VERSION 3.12)

project(containers)

add_executable(containers
     containers.cpp vector.cpp listsingle.cpp listdouble.cpp
)

set_target_properties(containers PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_compile_options(containers PRIVATE -Wall -Wextra -pedantic)