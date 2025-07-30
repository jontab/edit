cmake_minimum_required(VERSION 3.22)
project(edit-client-terminal LANGUAGES CXX C)

################################################################################
# termbox2                                                                     #
################################################################################

add_library(termbox2 INTERFACE)

target_include_directories(termbox2 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../deps/termbox2)

################################################################################
# edit-client-terminal                                                         #
################################################################################

add_library(edit-client-terminal-lib
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/BufferView.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/LayoutEngine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/StatusView.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/TermboxBackend.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/TermboxView.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BufferComponent.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Editor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/StatusComponent.cpp)

target_include_directories(edit-client-terminal-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_options(edit-client-terminal-lib PRIVATE -Wall -Werror)

target_link_libraries(edit-client-terminal-lib PUBLIC edit-common termbox2)

################################################################################
# edit                                                                         #
################################################################################

add_executable(edit-client-terminal ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

target_link_libraries(edit-client-terminal PRIVATE edit-client-terminal-lib)

################################################################################
# edit-tests                                                                   #
################################################################################

if(BUILD_TESTING)
    enable_testing()

    add_executable(edit-client-terminal-tests
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/BufferComponentTests.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/BusTests.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/EditorTests.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/StatusComponentTests.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/StatusViewTests.cpp)

    target_link_libraries(edit-client-terminal-tests
        edit-client-terminal-lib
        GTest::gtest_main)

    gtest_discover_tests(edit-client-terminal-tests)
endif()
