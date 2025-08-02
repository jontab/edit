cmake_minimum_required(VERSION 3.22)
project(edit-server LANGUAGES CXX C)

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
# edit-server-lib                                                              #
################################################################################

add_library(edit-server-lib 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BufferSessionManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FileSystemDatabase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/uWebSocketServer.cpp)

target_include_directories(edit-server-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_options(edit-server-lib PRIVATE -Wall -Werror -Wpedantic)

target_link_libraries(edit-server-lib PUBLIC uWebSockets edit-common)

################################################################################
# edit-server                                                                  #
################################################################################

add_executable(edit-server ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

target_link_libraries(edit-server PRIVATE edit-server-lib)

target_compile_options(edit-server PRIVATE -Wall -Werror -Wpedantic)

################################################################################
# edit-server-tests                                                            #
################################################################################

if(BUILD_TESTING)
    enable_testing()

    add_executable(edit-server-tests
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/BufferSessionManagerTests.cpp)

    target_link_libraries(edit-server-tests PRIVATE
        edit-server-lib 
        GTest::gmock_main)

    gtest_discover_tests(edit-server-tests)
endif()
