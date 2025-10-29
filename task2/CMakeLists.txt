cmake_minimum_required(VERSION 4.0)
project(task2)

set(CMAKE_CXX_STANDARD 17)

# Основное приложение
add_executable(task2
        src/main.cpp
        src/GameConfig.cpp
        src/Grid.cpp
        src/GameEngine.cpp
        src/LifeGame.cpp
        src/ConsoleInterface.cpp
        src/LifeFileWriter.cpp
        src/LifeParser.cpp
        src/LifeFileReader.cpp
        src/Command.cpp
        src/Command.h
)

# Подключаем Google Test
include(FetchContent)
FetchContent_Declare(
        googletest
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(test_game_of_life
        tst/TestGameEngine.cpp
        tst/TestConsoleInterface.cpp
        src/GameConfig.cpp
        src/Grid.cpp
        src/GameEngine.cpp
        src/LifeGame.cpp
        src/ConsoleInterface.cpp
        src/LifeFileWriter.cpp
        src/LifeParser.cpp
        src/LifeFileReader.cpp
        tst/TestGrid.cpp
        src/Command.cpp
        tst/TestLifeParser.cpp
)

target_include_directories(task2 PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_include_directories(test_game_of_life PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(test_game_of_life PRIVATE GTest::gtest GTest::gtest_main)

enable_testing()