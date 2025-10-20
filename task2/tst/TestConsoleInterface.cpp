#include "gtest/gtest.h"
#include "ConsoleInterface.h"

TEST(ConsoleInterfaceTest, ParseSimpleCommands) {
    const Cmd cmd1("exit");
    EXPECT_EQ(cmd1.getName(), "exit");
    EXPECT_EQ(cmd1.getAttribute(), "");

    const Cmd cmd2("help");
    EXPECT_EQ(cmd2.getName(), "help");
    EXPECT_EQ(cmd2.getAttribute(), "");

    const Cmd cmd3("tick");
    EXPECT_EQ(cmd3.getName(), "tick");
    EXPECT_EQ(cmd3.getAttribute(), "");
}

TEST(ConsoleInterfaceTest, ParseCommandsWithSpaces) {
    const Cmd cmd1("dump filename");
    EXPECT_EQ(cmd1.getName(), "dump");
    EXPECT_EQ(cmd1.getAttribute(), "filename");

    const Cmd cmd2("tick 5");
    EXPECT_EQ(cmd2.getName(), "tick");
    EXPECT_EQ(cmd2.getAttribute(), "5");

    const Cmd cmd3("auto 10");
    EXPECT_EQ(cmd3.getName(), "auto");
    EXPECT_EQ(cmd3.getAttribute(), "10");

    const Cmd cmd4("dump  my file with spaces ");
    EXPECT_EQ(cmd4.getName(), "dump");
    EXPECT_EQ(cmd4.getAttribute(), "my file with spaces");
}