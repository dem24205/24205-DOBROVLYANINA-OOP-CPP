#include <gtest/gtest.h>
#include "LifeParser.h"
#include "GameConfig.h"
#include <fstream>

void createTestFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    file << content;
    file.close();
}

void compareConfigFields(const GameConfig& actual, const GameConfig& expected) {
    EXPECT_EQ(actual.getNameUniverse(), expected.getNameUniverse());
    EXPECT_EQ(actual.getBirthRules(), expected.getBirthRules());
    EXPECT_EQ(actual.getSurvivalRules(), expected.getSurvivalRules());
    EXPECT_EQ(actual.getWidth(), expected.getWidth());
    EXPECT_EQ(actual.getHeight(), expected.getHeight());
    EXPECT_EQ(actual.getAliveCells(), expected.getAliveCells());
    EXPECT_EQ(actual.getMode(), expected.getMode());
    EXPECT_EQ(actual.getVersion(), expected.getVersion());
}

TEST(LifeParserTest, ParseValidConfigWithGlider) {
    createTestFile("test_glider.life", R"(#Life 1.06
#N Glider
#R B3/S23
#S C40/R40
0 0
1 1
1 2
2 0
2 1)");

    LifeParser parser("test_glider.life");
    char* argv[] = {(char*)"program", (char*)"test_glider.life"};
    GameConfig config = parser.handleParsing(2, argv);
    GameConfig expected;
    expected.setName("Glider");
    expected.setBirthRules({3});
    expected.setSurvivalRules({2, 3});
    expected.setSize(40, 40);
    expected.setGameVersion(GameVersion::Batch);
    expected.setGameMode(GameMode::Online);
    std::vector<std::pair<int, int>> expectedCells = {
        {0, 0}, {1, 1}, {1, 2}, {2, 0}, {2, 1}
    };
    expected.setAliveCells(expectedCells);
    compareConfigFields(config, expected);
    remove("test_glider.life");
}

TEST(LifeParserTest, ParseValidConfigWithBlockAndCustomRules) {
    createTestFile("test_block.life", R"(#Life 1.06
#N Block
#R B3678/S23
#S C20/R20
5 5
5 6
6 5
6 6)");

    LifeParser parser("test_block.life");
    char* argv[] = {(char*)"program", (char*)"test_block.life"};
    GameConfig config = parser.handleParsing(2, argv);
    GameConfig expected;
    expected.setName("Block");
    expected.setBirthRules({3, 6, 7, 8});
    expected.setSurvivalRules({2, 3});
    expected.setSize(20, 20);
    expected.setGameVersion(GameVersion::Batch);
    expected.setGameMode(GameMode::Online);
    std::vector<std::pair<int, int>> expectedCells = {
        {5, 5}, {5, 6}, {6, 5}, {6, 6}
    };
    expected.setAliveCells(expectedCells);
    compareConfigFields(config, expected);
    remove("test_block.life");
}

TEST(LifeParserTest, ParseInvalidFormatFile) {
    createTestFile("test_invalid.life", R"(#Life 1.05
Invalid format
1 1
2 2)");
    LifeParser parser("test_invalid.life");
    char* argv[] = {(char*)"program", (char*)"test_invalid.life"};
    GameConfig config = parser.handleParsing(2, argv);
    GameConfig expected;
    compareConfigFields(config, expected);
    remove("test_invalid.life");
}

TEST(LifeParserTest, ParseWithCommandLineArguments) {
    createTestFile("test_args.life", R"(#Life 1.06
#N Test
#R B3/S23
#C 30/30
1 1
2 2)");

    LifeParser parser("test_args.life");
    char* argv[] = {(char*)"program", (char*)"test_args.life", (char*)"--iterations=100", (char*)"--output=result.txt"};
    GameConfig config = parser.handleParsing(4, argv);
    EXPECT_EQ(config.getIterations(), 100);
    EXPECT_EQ(config.getOutputFile(), "result.txt");
    EXPECT_EQ(config.getMode(), GameMode::Offline);
    remove("test_args.life");
}

TEST(LifeParserTest, ParseNonExistentFile) {
    LifeParser parser("nonexistent.life");
    char* argv[] = {(char*)"program", (char*)"nonexistent.life"};
    GameConfig config = parser.handleParsing(2, argv);
    GameConfig expected;
    compareConfigFields(config, expected);
}

TEST(LifeParserTest, ParseDuplicateCells) {
    createTestFile("test_duplicates.life", R"(#Life 1.06
#N Test
#R B3/S23
#C 10/10
1 1
2 2
1 1
3 3)");

    LifeParser parser("test_duplicates.life");
    char* argv[] = {(char*)"program", (char*)"test_duplicates.life"};
    GameConfig config = parser.handleParsing(2, argv);
    std::vector<std::pair<int, int>> expectedCells = {
        {1, 1}, {2, 2}, {3, 3}
    };
    EXPECT_EQ(config.getAliveCells().size(), 3);
    EXPECT_EQ(config.getAliveCells(), expectedCells);
    remove("test_duplicates.life");
}

TEST(LifeParserTest, ParseIndividualMethods) {
    EXPECT_EQ(LifeParser::parseUniverseName("#N TestUniverse"), "TestUniverse");
    EXPECT_EQ(LifeParser::parseUniverseName("Invalid line"), "");
    EXPECT_EQ(LifeParser::parseBirthCondition("B3/S23"), std::vector<int>({3}));
    EXPECT_EQ(LifeParser::parseBirthCondition("B367/S238"), std::vector<int>({3, 6, 7}));
    EXPECT_EQ(LifeParser::parseBirthCondition("Invalid"), std::vector<int>({3})); // default
    EXPECT_EQ(LifeParser::parseSurvivalCondition("B3/S23"), std::vector<int>({2, 3}));
    EXPECT_EQ(LifeParser::parseSurvivalCondition("B3/S238"), std::vector<int>({2, 3, 8}));
    EXPECT_EQ(LifeParser::parseSurvivalCondition("Invalid"), std::vector<int>({2, 3})); // default
    auto [w2, h2] = LifeParser::parseGridSize("#C30/R40");
    EXPECT_EQ(w2, 30);
    EXPECT_EQ(h2, 40);
}

TEST(LifeParserTest, ParseInvalidRules) {
    createTestFile("test_bad_rules.life", R"(#Life 1.06
#N BadRules
#R B9/S99
#S C10/R10
1 1)");
    LifeParser parser("test_bad_rules.life");
    char* argv[] = {(char*)"program", (char*)"test_bad_rules.life"};
    GameConfig config = parser.handleParsing(2, argv);
    EXPECT_EQ(config.getBirthRules(), std::vector<int>({3}));
    EXPECT_EQ(config.getSurvivalRules(), std::vector<int>({2, 3}));
    remove("test_bad_rules.life");
}