#include <gtest/gtest.h>
#include <fstream>
#include "SoundProcessor.h"
#include "WAVHeader.h"

TEST(SoundProcessorIntegrationTest, PipelineWithMute) {
    std::string resultFile = "out_mute.wav";
    std::string inputFile1 = "funkorama.wav";
    std::string inputFile2 = "district_four.wav";
    std::string configFile = "test_config.txt";
    std::ofstream config(configFile);
    config << "mute 0 0\n";
    config << "mute 0 5";
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

TEST(SoundProcessorIntegrationTest, PipelineWithMix) {
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

TEST(SoundProcessorIntegrationTest, PipelineWithReverse) {
    std::string resultFile = "out_reverse.wav";
    std::string inputFile1 = "funkorama-[AudioTrimmer.com].wav";
    std::string inputFile2 = "district_four.wav";
    std::string configFile = "test_config.txt";
    std::ofstream config(configFile);
    config << "reverse 0 200";
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

TEST(SoundProcessorIntegrationTest, FullPipeline) {
    std::string resultFile = "processed_result.wav";
    std::string inputFile1 = "funkorama.wav";
    std::string inputFile2 = "district_four.wav";
    std::string configFile = "test_config.txt";
    std::ofstream config(configFile);
    config << "mute 0 5\n";
    config << "mix $2 0\n";
    config << "reverse 10 15\n";
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