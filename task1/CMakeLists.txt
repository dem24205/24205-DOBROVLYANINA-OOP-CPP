cmake_minimum_required(VERSION 3.16)
project(task1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(task1
        src/BitArray.cpp
        src/main.cpp
)

include(FetchContent)
FetchContent_Declare(
        googletest
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
include(GoogleTest)

target_include_directories(task1 PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_executable(test_bit_array
        tst/TestBitArray.cpp
        src/BitArray.cpp
)
target_include_directories(test_bit_array PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_bit_array PRIVATE GTest::gtest GTest::gtest_main)

gtest_discover_tests(test_bit_array)
