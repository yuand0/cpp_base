cmake_minimum_required(VERSION 3.10)
project(CppBase)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O0 -g -fPIC")

include(FetchContent)
FetchContent_Declare(googletest
    URL https://gitee.com/mirrors/googletest/repository/archive/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

add_library(intarray src/intarray.cpp)
target_include_directories(intarray PUBLIC include)

add_library(thread_pool src/thread_pool.cpp)
target_include_directories(thread_pool PUBLIC include)

add_library(thread_safe_stack src/thread_safe_stack.cpp)
target_include_directories(thread_safe_stack PUBLIC include)

add_library(tokenizer src/tokenizer.cpp)
target_include_directories(tokenizer PUBLIC include)

add_library(cosine src/cosine.cpp)
target_include_directories(cosine PUBLIC include)

add_library(calculator src/calculator.cpp)
target_include_directories(calculator PUBLIC include)

add_executable(test_intarray 
    tests/test_intarray.cpp 
    tests/test_thread_pool.cpp 
    tests/test_thread_safe_stack.cpp
    tests/test_tokenizer.cpp
    tests/test_calculator.cpp
)
target_link_libraries(test_intarray 
    intarray 
    thread_pool 
    thread_safe_stack 
    tokenizer 
    cosine
    calculator
    gtest_main
)

enable_testing()

add_subdirectory(pybind11)
pybind11_add_module(cpp_core python/bindings.cpp)
target_link_libraries(cpp_core PRIVATE intarray thread_safe_stack tokenizer calculator)

add_executable(calculator_benchmark benchmarks/calculator_benchmark.cpp)
target_link_libraries(calculator_benchmark calculator)

add_executable(calculator_benchmark benchmarks/calculator_benchmark.cpp)
target_link_libraries(calculator_benchmark calculator)
