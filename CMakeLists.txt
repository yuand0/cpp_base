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

add_executable(test_intarray tests/test_intarray.cpp)
target_link_libraries(test_intarray intarray gtest_main)

enable_testing()
