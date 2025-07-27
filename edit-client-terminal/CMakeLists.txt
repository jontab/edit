cmake_minimum_required(VERSION 3.22)
project(edit LANGUAGES CXX C)

include(${CMAKE_CURRENT_SOURCE_DIR}/../common.cmake)

add_subdirectory(
    ${CMAKE_CURRENT_SOURCE_DIR}/../edit-common
    ${CMAKE_CURRENT_BINARY_DIR}/edit-common-build)

################################################################################
# termbox2                                                                     #
################################################################################

add_library(termbox2 INTERFACE)

target_include_directories(termbox2 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../deps/termbox2)

################################################################################
# edit-client-terminal                                                         #
################################################################################

add_library(edit-client-terminal
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Editor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ViewContext.cpp)

target_include_directories(edit-client-terminal PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(edit-client-terminal PUBLIC edit-common termbox2)

################################################################################
# edit                                                                         #
################################################################################

add_executable(edit ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

target_link_libraries(edit PRIVATE edit-client-terminal)
