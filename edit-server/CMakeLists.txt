cmake_minimum_required(VERSION 3.22)
project(edit-server LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

################################################################################
# uSockets                                                                     #
################################################################################

set(uSockets_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../deps/uWebSockets/uSockets)

file(GLOB_RECURSE uSockets_SOURCES ${uSockets_SOURCE_DIR}/src/*.c)

add_library(uSockets ${uSockets_SOURCES})

target_include_directories(uSockets PUBLIC ${uSockets_SOURCE_DIR}/src)

target_compile_definitions(uSockets PRIVATE LIBUS_NO_SSL)

################################################################################
# uWebSockets                                                                  #
################################################################################

find_package(ZLIB REQUIRED)

set(uWebSockets_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../deps/uWebSockets)

add_library(uWebSockets INTERFACE)

target_include_directories(uWebSockets INTERFACE ${uWebSockets_SOURCE_DIR}/src)

target_link_libraries(uWebSockets INTERFACE uSockets ${ZLIB_LIBRARIES})

################################################################################
# edit-server                                                                  #
################################################################################

add_executable(edit-server ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

target_link_libraries(edit-server uWebSockets)

target_compile_options(edit-server PRIVATE -Wall -Werror -Wpedantic)
