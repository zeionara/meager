cmake_minimum_required(VERSION 3.16.3)

project(meager)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -pthread -march=native")
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/release)

set(
    SOURCES
    base/main.cpp base/Setting.cpp base/Random.cpp base/Reader.cpp base/Test.cpp base/Corrupt.cpp base/Utils.cpp
    # patterns
    base/patterns/main.cpp
    base/patterns/symmetric/reader.cpp base/patterns/symmetric/main.cpp 
    base/patterns/none/reader.cpp base/patterns/none/main.cpp
    base/patterns/inverse/reader.cpp base/patterns/inverse/main.cpp

    base/filters/reader.cpp base/filters/main.cpp
)
set(
    ERLANG_SOURCES
    erlang/utils.cpp erlang/setting.cpp erlang/main.cpp erlang/random.cpp erlang/reader.cpp erlang/test.cpp
)
set(
    PYTHON_SOURCES
    python/main.cpp python/setting.cpp python/random.cpp python/reader.cpp python/test.cpp
)

include_directories(/usr/lib/erlang/usr/include)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

add_library(meager ${SOURCES})
add_library(meager_erlang ${ERLANG_SOURCES})
add_library(meager_python ${PYTHON_SOURCES})

target_link_libraries(meager_erlang meager)
target_link_libraries(meager_python meager)

install(TARGETS meager meager_erlang meager_python DESTINATION /usr/lib/)

