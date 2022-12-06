cmake_minimum_required(VERSION 3.16.3)

project(meagerTests)

set(CMAKE_CXX_STANDARD 20)
set(LIBRARY_PATH ${CMAKE_BINARY_DIR}/release)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

function (test name)
    add_executable(${name} ${name}.cpp)
    add_test(NAME ${name} COMMAND ${name})
endfunction()

enable_testing()

test(stopwatch)
