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
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/BufferView.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/LayoutEngine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/StatusView.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/TermboxBackend.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/TermboxView.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BufferComponent.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Editor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/StatusComponent.cpp)

target_include_directories(edit-client-terminal PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_options(edit-client-terminal PRIVATE -Wall -Werror)

target_link_libraries(edit-client-terminal PUBLIC edit-common termbox2)

################################################################################
# edit                                                                         #
################################################################################

add_executable(edit ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

target_link_libraries(edit PRIVATE edit-client-terminal)

################################################################################
# edit-tests                                                                   #
################################################################################

if(BUILD_TESTING)
    enable_testing()

    add_executable(edit-tests
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/BufferComponentTests.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/BusTests.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/EditorTests.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/StatusComponentTests.cpp)

    target_link_libraries(edit-tests
        edit-client-terminal
        GTest::gtest_main)

    include(GoogleTest)
    gtest_discover_tests(edit-tests)
endif()
