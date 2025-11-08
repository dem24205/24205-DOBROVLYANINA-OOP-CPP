#include <gtest/gtest.h>
#include <fstream>
#include "SoundProcessor.h"
#include "WAVHeader.h"

TEST(SoundProcessorIntegrationTest, FullPipelineWithMute) {
    std::string resultFile = "out_mute.wav";
    std::string inputFile1 = "funkorama.wav";
    std::string inputFile2 = "district_four.wav";
    std::string configFile = "test_config.txt";
    std::ofstream config(configFile);
    config << "mute 0 5\n";
    config << "mute 25 30";
    config.close();
    std::vector inputFiles = {resultFile, inputFile1, inputFile2};
    SoundProcessor processor(configFile, inputFiles);
    EXPECT_NO_THROW(processor.run());
    WAVHeaderParser parser;
    EXPECT_NO_THROW(parser.parse(resultFile));
    std::ifstream test(resultFile, std::ios::binary | std::ios::ate);
    EXPECT_TRUE(test.is_open());
    test.close();
}

TEST(SoundProcessorIntegrationTest, FullPipelineWithMix) {
    std::string resultFile = "out_mixed.wav";
    std::string inputFile1 = "funkorama.wav";
    std::string inputFile2 = "district_four.wav";
    std::string configFile = "test_config.txt";
    std::ofstream config(configFile);
    config << "mix $2 0";
    config.close();
    std::vector inputFiles = {resultFile, inputFile1, inputFile2};
    SoundProcessor processor(configFile, inputFiles);
    EXPECT_NO_THROW(processor.run());
    WAVHeaderParser parser;
    EXPECT_NO_THROW(parser.parse(resultFile));
    std::ifstream test(resultFile, std::ios::binary | std::ios::ate);
    EXPECT_TRUE(test.is_open());
    test.close();
}

TEST(SoundProcessorIntegrationTest, FullPipelineWithMuteAndMix) {
    std::string resultFile = "processed_result.wav";
    std::string inputFile1 = "funkorama.wav";
    std::string inputFile2 = "district_four.wav";
    std::string configFile = "test_config.txt";
    std::ofstream config(configFile);
    config << "mute 0 5\n";
    config << "mix $2 5\n";
    config << "mute 25 30";
    config.close();
    std::vector inputFiles = {resultFile, inputFile1, inputFile2};
    SoundProcessor processor(configFile, inputFiles);
    EXPECT_NO_THROW(processor.run());
    WAVHeaderParser parser;
    EXPECT_NO_THROW(parser.parse(resultFile));
    std::ifstream test(resultFile, std::ios::binary | std::ios::ate);
    EXPECT_TRUE(test.is_open());
    test.close();
}