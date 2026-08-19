cmake_minimum_required(VERSION 3.10)
project(CppBase)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O0 -g")

include(FetchContent)
FetchContent_Declare(googletest
    URL https://gitee.com/mirrors/googletest/repository/archive/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

add_library(intarray src/intarray.cpp)
target_include_directories(intarray PUBLIC include)

add_library(thread_pool src/thread_pool.cpp)
target_include_directories(thread_pool PUBLIC include)

add_executable(test_intarray tests/test_intarray.cpp tests/test_thread_pool.cpp)
target_link_libraries(test_intarray intarray thread_pool gtest_main)

add_executable(word_counter benchmarks/word_counter.cpp)
target_link_libraries(word_counter thread_pool)

enable_testing()

# pybind11 绑定
add_subdirectory(pybind11)
pybind11_add_module(cpp_core python/bindings.cpp)
target_link_libraries(cpp_core PRIVATE intarray)
