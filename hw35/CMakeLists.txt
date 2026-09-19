cmake_minimum_required(VERSION 3.7.2)
project(topk_words CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(topk_words_single topk_words.cpp)
# add_executable(topk_words_par1 topk_words_par1.cpp)

target_compile_options(topk_words_single PUBLIC -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion)
# target_compile_options(topk_words_par1 PUBLIC -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion)
