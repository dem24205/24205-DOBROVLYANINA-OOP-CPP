#include <gtest/gtest.h>
#include "ConsoleParser.h"
#include "SoundProcessorException.h"

TEST(ConsoleHandlerTest, HelpMode) {
    ConsoleParser handler;
    char* argv[] = {(char *)"program", (char *)"-h"};
    constexpr int argc = 2;
    EXPECT_EQ(handler.parse(argc, argv), RunMode::Help);
}

TEST(ConsoleHandlerTest, HelpModeWithExtraArgsThrows) {
    ConsoleParser handler;
    char* argv[] = {(char*)"program", (char*)"-h", (char*)"extra"};
    constexpr int argc = 3;
    EXPECT_THROW(handler.parse(argc, argv), ConsoleException);
}

TEST(ConsoleHandlerTest, RunModeValid) {
    ConsoleParser handler;
    char* argv[] = {(char*)"program", (char*)"-c", (char*)"config.txt",
                    (char*)"output.wav", (char*)"input1.wav", (char*)"input2.wav"};
    constexpr int argc = 6;
    EXPECT_EQ(handler.parse(argc, argv), RunMode::Run);
    EXPECT_EQ(handler.getConfigFilename(), "config.txt");
    const auto filenames = handler.getFilenames();
    ASSERT_EQ(filenames.size(), 3);
    EXPECT_EQ(filenames[0], "output.wav");
    EXPECT_EQ(filenames[1], "input1.wav");
    EXPECT_EQ(filenames[2], "input2.wav");
}
