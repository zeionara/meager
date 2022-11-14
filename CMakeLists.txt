cmake_minimum_required(VERSION 3.16.3)

project(meager)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -pthread -march=native")
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/release)

file(
    GLOB_RECURSE SOURCES_FILES "base/*.cpp"
)
file(
    GLOB_RECURSE ERLANG_SOURCES_FILES "erlang/*.cpp"
)
file(
    GLOB_RECURSE PYTHON_SOURCES_FILES "python/*.cpp"
)

set(
    SOURCES
    ${SOURCES_FILES}
)
set(
    ERLANG_SOURCES
    ${ERLANG_SOURCES_FILES}
)
set(
    PYTHON_SOURCES
    ${PYTHON_SOURCES_FILES}
)

include_directories(/usr/lib/erlang/usr/include)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

add_library(meager ${SOURCES})
add_library(meager_erlang ${ERLANG_SOURCES})
add_library(meager_python ${PYTHON_SOURCES})

target_link_libraries(meager_erlang meager)
target_link_libraries(meager_python meager)

install(TARGETS meager meager_erlang meager_python DESTINATION /usr/lib/)

