cmake_minimum_required(VERSION 3.22)
project(edit-common LANGUAGES CXX)


################################################################################
# nlohmann-json                                                                #
################################################################################

add_library(nlohmann-json INTERFACE)

target_include_directories(nlohmann-json INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../deps/json/single_include)

################################################################################
# edit-common                                                                 #
################################################################################

add_library(edit-common 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Buffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Char.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Delta.cpp)

target_include_directories(edit-common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_options(edit-common PRIVATE -Wall -Werror -Wpedantic)

target_link_libraries(edit-common PUBLIC nlohmann-json)

################################################################################
# edit-common-tests                                                            #
################################################################################

if(BUILD_TESTING)
    enable_testing()

    add_executable(edit-common-tests
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/BufferTests.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/CharTests.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/DeltaTests.cpp)

    target_link_libraries(edit-common-tests
        edit-common
        GTest::gtest_main)

    target_include_directories(edit-common-tests PRIVATE nlohmann-json)

    gtest_discover_tests(edit-common-tests)
endif()
