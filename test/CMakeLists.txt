cmake_minimum_required(VERSION 3.16.3)

project(meagerTests)

set(CMAKE_CXX_STANDARD 20)
set(LIBRARY_PATH ${CMAKE_BINARY_DIR}/release)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# file(
#     GLOB wtf "*.cpp"
# )

# message(${wtf})

function (test name)
    add_executable(${name} ${name}.cpp)
    add_test(NAME ${name} COMMAND ${name})
endfunction()
# add_executable(
#     foo
#     ${wtf}
# )

enable_testing()

test(stopwatch)

# foreach(X IN LISTS A B C D E)
#     message(STATUS "X=${X}")

# foreach(FILE IN ITEMS ${SOURCE_FILES})
#     get_filename_component(FILENAME ${FILE} NAME)
#     add_executable(${FILENAME} ${FILE})
#     add_test(NAME ${FILE} COMMAND ${FILENAME})
# endforeach()

# add_test(NAME meagerTests COMMAND foo)
# add_test(NAME meagerTests COMMAND SOURCESA)

# include_directories(/home/zeio/randeer/randeer)
# 
# option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
# 
# add_library(meager ${SOURCES})
# add_library(meager_erlang ${ERLANG_SOURCES})
# add_library(meager_python ${PYTHON_SOURCES})
# 
# add_library(randeer SHARED IMPORTED)
# set_target_properties(randeer PROPERTIES IMPORTED_LOCATION /usr/lib/librandeer.so)
# 
# target_link_libraries(meager_erlang meager)
# target_link_libraries(meager_python meager)
# target_link_libraries(meager randeer)
# 
# install(TARGETS meager meager_erlang meager_python DESTINATION /usr/lib/)
