cmake_minimum_required(VERSION 3.16)
project(task0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(task0
        src/main.cpp
        src/FileReader.cpp
        src/CSVFileWriter.cpp
        src/TextParser.cpp
        src/WordFrequencyAnalyzer.cpp
        src/WordStat.cpp
        src/WordStat.h
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

add_executable(test_parser
        tst/TestTextParser.cpp
        src/TextParser.cpp
)
target_include_directories(task0 PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_include_directories(test_parser PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_parser PRIVATE GTest::gtest GTest::gtest_main)

add_executable(test_analyzer
        tst/TestWordFrequencyAnalyzer.cpp
        src/WordFrequencyAnalyzer.cpp
        src/WordStat.cpp
)
target_include_directories(test_analyzer PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_analyzer PRIVATE GTest::gtest GTest::gtest_main)

gtest_discover_tests(test_parser)
gtest_discover_tests(test_analyzer)