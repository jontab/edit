cmake_minimum_required(VERSION 3.22)
project(edit-common LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        52eb8108c5bdec04579160ae17225d66034bd723 # release-1.17.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

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

enable_testing()

add_executable(edit-common-tests
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/BufferTests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/CharTests.cpp)

target_link_libraries(edit-common-tests
    edit-common
    GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(edit-common-tests)
