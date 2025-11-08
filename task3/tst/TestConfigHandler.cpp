#include <gtest/gtest.h>
#include <sstream>
#include <fstream>
#include "ConfigHandler.h"
#include "SoundProcessorException.h"

using namespace std;

TEST(ConfigCommandTest, MuteCommandCreationAndGetters) {
    const MuteCommand command("0 10");
    EXPECT_EQ(command.getName(), "mute");
    EXPECT_EQ(command.getStart(), 0);
    EXPECT_EQ(command.getEnd(), 10);
}

TEST(ConfigCommandTest, MixCommandCreationAndGetters) {
    MixCommand command("$1 40");
    EXPECT_EQ(command.getName(), "mix");
    EXPECT_EQ(command.getStart(), 40);
    EXPECT_EQ(command.getInputFileIdx(), 1);
}

TEST(CommandFactoryTest, FactoryCreation) {
    const CommandFactory factory;
    const auto muteCmd = factory.createCommand("mute", "0 1000");
    ASSERT_NE(muteCmd, nullptr);
    EXPECT_EQ(muteCmd->getName(), "mute");

    const auto mixCmd = factory.createCommand("mix", "$1 500");
    ASSERT_NE(mixCmd, nullptr);
    EXPECT_EQ(mixCmd->getName(), "mix");
}

TEST(CommandFactoryTest, FactoryUnknownCommand) {
    const CommandFactory factory;

    const auto unknownCmd = factory.createCommand("unknown", "attr");
    EXPECT_EQ(unknownCmd, nullptr);

    const auto emptyCmd = factory.createCommand("", "attr");
    EXPECT_EQ(emptyCmd, nullptr);
}

TEST(CommandFactoryTest, ConcreteCommandTypes) {
    const CommandFactory factory;
    const auto muteCmd = factory.createCommand("mute", "10 20");
    const auto mixCmd = factory.createCommand("mix", "$2");

    auto* muteConcrete = dynamic_cast<MuteCommand*>(muteCmd.get());
    auto* mixConcrete = dynamic_cast<MixCommand*>(mixCmd.get());
    ASSERT_NE(muteConcrete, nullptr);
    ASSERT_NE(mixConcrete, nullptr);

    EXPECT_EQ(muteConcrete->getStart(), 10);
    EXPECT_EQ(muteConcrete->getEnd(), 20);
    EXPECT_EQ(mixConcrete->getStart(), 0);
    EXPECT_EQ(mixConcrete->getInputFileIdx(), 2);
}

TEST(CommandCreatorsTest, MuteCommandCreator) {
    const MuteCommandCreator creator;
    EXPECT_EQ(creator.getCommandName(), "mute");

    const auto command = creator.createCommand("50 150");
    ASSERT_NE(command, nullptr);
    EXPECT_EQ(command->getName(), "mute");

    auto* muteCmd = dynamic_cast<MuteCommand*>(command.get());
    ASSERT_NE(muteCmd, nullptr);
    EXPECT_EQ(muteCmd->getStart(), 50);
    EXPECT_EQ(muteCmd->getEnd(), 150);
}

TEST(CommandCreatorsTest, MixCommandCreator) {
    const MixCommandCreator creator;
    EXPECT_EQ(creator.getCommandName(), "mix");

    const auto command = creator.createCommand("$3 20");
    ASSERT_NE(command, nullptr);
    EXPECT_EQ(command->getName(), "mix");

    auto* mixCmd = dynamic_cast<MixCommand*>(command.get());
    ASSERT_NE(mixCmd, nullptr);
    EXPECT_EQ(mixCmd->getStart(), 20);
    EXPECT_EQ(mixCmd->getInputFileIdx(), 3);
}

TEST(ConfigHandlerTest, ReadCommandsFromStream) {
    ofstream tempFile("test_config.txt");
    tempFile << "mute 0 10\n";
    tempFile << "mix $1 20\n";
    tempFile << "mute\n";
    tempFile.close();

    ifstream configStream("test_config.txt");
    ConfigHandler handler(&configStream);
    auto cmd1 = handler.getCommand();
    ASSERT_NE(cmd1, nullptr);
    EXPECT_EQ(cmd1->getName(), "mute");

    auto cmd2 = handler.getCommand();
    ASSERT_NE(cmd2, nullptr);
    EXPECT_EQ(cmd2->getName(), "mix");

    EXPECT_THROW(handler.getCommand(), ConfigException);
    configStream.close();
    remove("test_config.txt");
}

TEST(ConfigHandlerTest, InvalidCommandInStream) {
    ofstream tempFile("invalid_test.txt");
    tempFile << "unknown 0 1000\n";
    tempFile.close();
    ifstream configStream("invalid_test.txt");
    ConfigHandler handler(&configStream);
    EXPECT_THROW(handler.getCommand(), ConfigException);
    configStream.close();
    remove("invalid_test.txt");
}

TEST(ConfigHandlerTest, CommentsMixedWithInvalidArgs) {
    std::ofstream tempFile("comment_test.txt");
    tempFile << "# this is a comment\n";
    tempFile << "\n";  // Пустая строка
    tempFile << "mute 12\n";
    tempFile << "mix\n";
    tempFile.close();

    std::ifstream configStream("comment_test.txt");
    ConfigHandler handler(&configStream);

    auto cmd1 = handler.getCommand();
    ASSERT_NE(cmd1, nullptr);
    EXPECT_EQ(cmd1->getName(), "comment");

    auto cmd2 = handler.getCommand();
    ASSERT_NE(cmd2, nullptr);
    EXPECT_EQ(cmd2->getName(), "comment");
    //last two invalid cmds
    EXPECT_THROW(handler.getCommand(), ConfigException);
    EXPECT_THROW(handler.getCommand(), ConfigException);
    configStream.close();
    remove("comment_test.txt");
}