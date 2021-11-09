cmake_minimum_required(VERSION 3.16.3)

project(meager)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -pthread -march=native")
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/release)
set(SOURCES base/main.cpp base/erlang.cpp base/Setting.cpp)

include_directories(/usr/lib/erlang/erts-12.1.3/include)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

add_library(meager ${SOURCES})

install(TARGETS meager DESTINATION /usr/lib/)

