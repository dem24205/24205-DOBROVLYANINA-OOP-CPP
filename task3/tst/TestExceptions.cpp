#include "ConfigHandler.h"
#include "ConsoleParser.h"
#include "SoundProcessor.h"
#include "SoundProcessorException.h"
#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include <filesystem>

TEST(SoundProcessorExceptionTest, InvalidConfigCommand) {
    std::string brokenConfigFile = "invalid_command.txt";
    std::ofstream config(brokenConfigFile);
    config << "invalid_command 0 5" << std::endl;
    config.close();
    std::vector<std::string> inputFiles = {"output.wav", "funkorama.wav"};
    SoundProcessor processor(brokenConfigFile, inputFiles);
    EXPECT_THROW(processor.run(), ConfigException);
}

TEST(ConsoleHandlerExceptionTest, RunModeNotEnoughArguments) {
    ConsoleParser handler;
    char* argv[] = {const_cast<char *>("program_name"), const_cast<char *>("-c"), const_cast<char *>("config.txt")};
    EXPECT_THROW(handler.parse(3, argv), ConsoleException);
}

