cmake_minimum_required(VERSION 4.0)
project(task4)

set(CMAKE_CXX_STANDARD 17)


add_executable(task4 src/main.cpp
        src/CSVParser.h)
target_include_directories(task4 PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Google Test
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

add_executable(test_tuple_printer
        tst/TestTuplePrinter.cpp
        tst/TestCSVParser.cpp
        tst/TestImprovedCSVParser.cpp
)

target_include_directories(test_tuple_printer PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_tuple_printer PRIVATE GTest::gtest GTest::gtest_main)

gtest_discover_tests(test_tuple_printer)