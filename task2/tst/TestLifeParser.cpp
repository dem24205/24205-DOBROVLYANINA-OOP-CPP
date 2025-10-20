#include "gtest/gtest.h"
#include "LifeParser.h"
#include "GameConfig.h"
#include <fstream>

// simple valid file
TEST(LifeParserTest, ValidFile) {
    std::ofstream file("test.life");
    file << "#Life 1.06\n";
    file << "#N Glider\n";
    file << "#R B3/S23\n";
    file << "#S C10/R10\n";
    file << "1 2\n";
    file << "2 3\n";
    file.close();
    char* argv[] = {(char*)"program", (char*)"test.life"};
    int argc = 2;
    LifeParser parser(argc, argv);
    GameConfig config;
    parser.handleParsing(config, argc, argv);
    EXPECT_EQ(config.getNameUniverse(), "Glider");
    EXPECT_EQ(config.getWidth(), 10);
    EXPECT_EQ(config.getHeight(), 10);
    EXPECT_EQ(config.getAliveCells().size(), 2);
    std::remove("test.life");
}

// invalid format file -> set random pattern
TEST(LifeParserTest, InvalidFile) {
    std::ofstream file("test_bad.life");
    file << "#Life 1.05\n";
    file.close();
    char* argv[] = {(char*)"program", (char*)"test_bad.life"};
    int argc = 2;
    LifeParser parser(argc, argv);
    GameConfig config;
    parser.handleParsing(config, argc, argv);

    EXPECT_EQ(config.getVersion(), GameVersion::Demo);

    std::remove("test_bad.life");
}

// empty universe -> random grid pattern
TEST(LifeParserTest, EmptyCellsFile) {
    std::ofstream file("test_empty.life");
    file << "#Life 1.06\n";
    file << "#N Empty\n";
    file << "#R B5/S78\n";
    file << "#S C5/R5\n";
    file.close();

    char* argv[] = {(char*)"program", (char*)"test_empty.life"};
    int argc = 2;

    LifeParser parser(argc, argv);
    GameConfig config;
    parser.handleParsing(config, argc, argv);

    EXPECT_EQ(config.getVersion(), GameVersion::Demo);
    EXPECT_EQ(config.getNameUniverse(), "Empty");
    EXPECT_EQ(config.getBirthRules(), std::vector<int>({5}));
    EXPECT_EQ(config.getSurvivalRules(), std::vector<int>({7, 8}));
    EXPECT_EQ(config.getWidth(), 5);
    EXPECT_EQ(config.getHeight(), 5);
    EXPECT_TRUE(config.getAliveCells().empty());
    std::remove("test_empty.life");
}

// simple valid argv
TEST(LifeParserTest, WithCommandLineArgs) {
    std::ofstream file("test.life");
    file << "#Life 1.06\n";
    file << "#N Test\n";
    file << "#R B3/S23\n";
    file << "#S C8/R8\n";
    file << "1 1\n";
    file.close();

    char* argv[] = {(char*)"program", (char*)"test.life",
                   (char*)"--iterations=50", (char*)"--output=result.txt"};
    int argc = 4;

    LifeParser parser(argc, argv);
    GameConfig config;
    parser.handleParsing(config, argc, argv);

    // ПРАВИЛЬНЫЕ ПРОВЕРКИ:
    EXPECT_EQ(config.getMode(), GameMode::Offline);
    EXPECT_EQ(config.getVersion(), GameVersion::Batch);
    EXPECT_EQ(config.getIterations(), 50);
    EXPECT_EQ(config.getOutputFile(), "result.txt");
    EXPECT_EQ(config.getNameUniverse(), "Test");
    EXPECT_EQ(config.getBirthRules(), std::vector<int>({3}));
    EXPECT_EQ(config.getSurvivalRules(), std::vector<int>({2, 3}));
    EXPECT_EQ(config.getWidth(), 8);
    EXPECT_EQ(config.getHeight(), 8);
    EXPECT_EQ(config.getAliveCells().size(), 1);
    std::remove("test.life");
}

TEST(LifeParserTest, NoFile) {
    char* argv[] = {(char*)"program"};
    int argc = 1;
    LifeParser parser(argc, argv);
    GameConfig config;
    parser.handleParsing(config, argc, argv);
    EXPECT_EQ(config.getMode(), GameMode::Online);
    EXPECT_EQ(config.getVersion(), GameVersion::Demo);
    EXPECT_EQ(config.getNameUniverse(), "Random Universe");
    EXPECT_EQ(config.getBirthRules(), std::vector<int>({3}));
    EXPECT_EQ(config.getSurvivalRules(), std::vector<int>({2, 3}));
    EXPECT_EQ(config.getWidth(), 35);
    EXPECT_EQ(config.getHeight(), 35);
    EXPECT_EQ(config.getIterations(), 1);
    EXPECT_EQ(config.getOutputFile(), "output");
    EXPECT_TRUE(config.getAliveCells().empty());
}