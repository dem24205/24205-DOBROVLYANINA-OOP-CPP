cmake_minimum_required(VERSION 4.0)
project(task3)

set(CMAKE_CXX_STANDARD 17)

# Основное приложение
add_executable(task3
        src/main.cpp
        src/WAVHeader.cpp
        src/WAVHeader.h
        src/SoundProcessorException.cpp
        src/SoundProcessorException.h
        src/ConfigHandler.cpp
        src/ConfigHandler.h
        src/Converters.cpp
        src/Converters.h
        src/ConsoleHandler.cpp
        src/ConsoleHandler.h
)

target_include_directories(task3 PRIVATE ${CMAKE_SOURCE_DIR}/src)

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

# Тестовое приложение - ДОБАВЬТЕ ConsoleHandler.cpp и другие необходимые файлы
add_executable(test_sound_processor
        tst/TestConsoleHandler.cpp
        src/ConsoleHandler.cpp
        src/ConsoleHandler.h
        src/SoundProcessorException.cpp
        src/SoundProcessorException.h
)

target_include_directories(test_sound_processor PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_sound_processor PRIVATE GTest::gtest GTest::gtest_main)

gtest_discover_tests(test_sound_processor)