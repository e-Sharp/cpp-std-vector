cmake_minimum_required(VERSION 3.0.0)

project(cpp-std-vector)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_executable(tests)

target_include_directories(tests
    PUBLIC inc
)

target_sources(tests
    PUBLIC src/tests.cpp
)
