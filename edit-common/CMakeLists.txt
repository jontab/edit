cmake_minimum_required(VERSION 3.22)
project(edit-common LANGUAGES CXX)

include(${CMAKE_CURRENT_SOURCE_DIR}/../common.cmake)

################################################################################
# edit-common                                                                 #
################################################################################

add_library(edit-common 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Buffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Char.cpp)

target_include_directories(edit-common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_options(edit-common PRIVATE -Wall -Werror -Wpedantic)

################################################################################
# edit-common-tests                                                            #
################################################################################

if(BUILD_TESTING)
    enable_testing()

    add_executable(edit-common-tests
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/BufferTests.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/CharTests.cpp)

    target_link_libraries(edit-common-tests
        edit-common
        GTest::gtest_main)

    include(GoogleTest)
    gtest_discover_tests(edit-common-tests)
endif()
