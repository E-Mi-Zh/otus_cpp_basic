cmake_minimum_required(VERSION 3.23)

project(containers_tests)

# Build googletest
add_subdirectory(
    "${CMAKE_CURRENT_SOURCE_DIR}/googletest" 
    "googletest"
)

# Test target
add_executable(
    containers_tests
    tests/vector_tests.cpp
    tests/list_tests.cpp
    src/vector.cpp
    src/listsingle.cpp
)

# Add src to the include directories for the test target
target_include_directories(
    containers_tests PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# Link test target against gtest libraries
target_link_libraries(
    containers_tests PRIVATE
    gtest gtest_main
)

if (MSVC)
    # @see https://github.com/google/googletest/blob/master/googletest/README.md
    # Visual Studio Dynamic vs Static Runtimes
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    target_compile_options(
        containers_tests PRIVATE
        /W4
    )
else ()
    target_compile_options(
        containers_tests PRIVATE
        -Wall -Wextra -pedantic -Werror -g
    )
endif()