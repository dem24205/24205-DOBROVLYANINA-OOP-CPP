#include <gtest/gtest.h>
#include "ConsoleHandler.h"
#include "SoundProcessorException.h"

TEST(ConsoleHandlerTest, HelpMode) {
    ConsoleHandler handler;
    char* argv[] = {(char *)"program", (char *)"-h"};
    constexpr int argc = 2;
    EXPECT_EQ(handler.parseConsoleCommand(argc, argv), RunMode::Help);
}

TEST(ConsoleHandlerTest, HelpModeWithExtraArgsThrows) {
    ConsoleHandler handler;
    char* argv[] = {(char*)"program", (char*)"-h", (char*)"extra"};
    constexpr int argc = 3;
    EXPECT_THROW(handler.parseConsoleCommand(argc, argv), ConfigException);
}

TEST(ConsoleHandlerTest, RunModeValid) {
    ConsoleHandler handler;
    char* argv[] = {(char*)"program", (char*)"-c", (char*)"config.txt",
                    (char*)"output.wav", (char*)"input1.wav", (char*)"input2.wav"};
    constexpr int argc = 6;
    EXPECT_EQ(handler.parseConsoleCommand(argc, argv), RunMode::Run);
    EXPECT_EQ(handler.getConfigFilename(), "config.txt");
    const auto filenames = handler.getFilenames();
    ASSERT_EQ(filenames.size(), 3);
    EXPECT_EQ(filenames[0], "output.wav");
    EXPECT_EQ(filenames[1], "input1.wav");
    EXPECT_EQ(filenames[2], "input2.wav");
}
