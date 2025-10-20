#include "gtest/gtest.h"
#include "GameEngine.h"
#include "GameConfig.h"

// simple demo engine
TEST(GameEngineTest, DemoVersionCreatesRandomPattern) {
    GameConfig config;
    config.setGameVersion(GameVersion::Demo);
    config.setSize(50, 50);
    GameEngine engine(config);
    EXPECT_EQ(engine.getBirthRules(), std::vector<int>({3}));
    EXPECT_EQ(engine.getSurvivalRules(), std::vector<int>({2, 3}));
    EXPECT_EQ(engine.getWidth(), 50);
    EXPECT_EQ(engine.getHeight(), 50);
    auto grid = engine.getGrid();
    bool hasAliveCells = false;
    for (const auto& row : grid) {
        for (bool cell : row) {
            if (cell) {
                hasAliveCells = true;
                break;
            }
        }
        if (hasAliveCells) break;
    }
    EXPECT_TRUE(hasAliveCells);
}

// simple batch engine
TEST(GameEngineTest, BatchVersionWithCustomCells) {
    GameConfig config;
    config.setGameVersion(GameVersion::Batch);
    config.setSize(10, 10);
    config.setAliveCells({{1, 1}, {2, 2}, {3, 3}});
    config.setBirthRules({3});
    config.setSurvivalRules({2, 3});
    config.setName("Test Universe");
    GameEngine engine(config);
    EXPECT_EQ(engine.getUniverseName(), "Test Universe");
    EXPECT_EQ(engine.getBirthRules(), std::vector<int>({3}));
    EXPECT_EQ(engine.getSurvivalRules(), std::vector<int>({2, 3}));

    auto grid = engine.getGrid();
    EXPECT_TRUE(grid[1][1]);  // клетка (1,1) должна быть живой
    EXPECT_TRUE(grid[2][2]);  // клетка (2,2) должна быть живой
    EXPECT_TRUE(grid[3][3]);  // клетка (3,3) должна быть живой
    EXPECT_FALSE(grid[0][0]); // клетка (0,0) должна быть мертвой
}

TEST(GameEngineTest, NeighborCounting) {
    GameConfig config;
    config.setGameVersion(GameVersion::Batch);
    config.setSize(5, 5);
    config.setAliveCells({{1, 2}, {2, 2}, {3, 2}});
    GameEngine engine(config);
    EXPECT_EQ(engine.countNeighbors(2, 2), 2);
    EXPECT_EQ(engine.countNeighbors(1, 2), 1);
    EXPECT_EQ(engine.countNeighbors(0, 2), 1);
    EXPECT_EQ(engine.countNeighbors(4, 2), 1);
}

TEST(GameEngineTest, EvolutionWithStandardRules) {
    GameConfig config;
    config.setGameVersion(GameVersion::Batch);
    config.setSize(5, 5);
    config.setAliveCells({{1, 1}, {1, 2}, {2, 1}, {2, 2}});
    config.setBirthRules({3});
    config.setSurvivalRules({2, 3});
    GameEngine engine(config);
    engine.countNextGeneration();
    auto grid = engine.getGrid();
    EXPECT_TRUE(grid[1][1]);
    EXPECT_TRUE(grid[1][2]);
    EXPECT_TRUE(grid[2][1]);
    EXPECT_TRUE(grid[2][2]);
    EXPECT_FALSE(grid[0][0]);
    EXPECT_FALSE(grid[0][1]);
    EXPECT_FALSE(grid[0][2]);
    EXPECT_FALSE(grid[0][3]);
    EXPECT_FALSE(grid[1][0]);
    EXPECT_FALSE(grid[1][3]);
    EXPECT_FALSE(grid[2][0]);
    EXPECT_FALSE(grid[2][3]);
    EXPECT_FALSE(grid[3][0]);
    EXPECT_FALSE(grid[3][1]);
    EXPECT_FALSE(grid[3][2]);
    EXPECT_FALSE(grid[3][3]);
}

// Blinker
TEST(GameEngineTest, MultipleGenerations) {
    GameConfig config;
    config.setGameVersion(GameVersion::Batch);
    config.setSize(20, 20);
    config.setAliveCells({{10, 11}, {11, 11}, {12, 11}});
    config.setBirthRules({3});
    config.setSurvivalRules({2, 3});
    GameEngine engine(config);
    //vertical
    engine.countNextGeneration();
    auto grid1 = engine.getGrid();
    EXPECT_TRUE(grid1[10][11]);
    EXPECT_TRUE(grid1[11][11]);
    EXPECT_TRUE(grid1[12][11]);
    //horizontal
    engine.countNextGeneration();
    auto grid2 = engine.getGrid();
    EXPECT_TRUE(grid2[11][10]);
    EXPECT_TRUE(grid2[11][11]);
    EXPECT_TRUE(grid2[11][12]);
}