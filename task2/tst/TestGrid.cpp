#include "gtest/gtest.h"
#include "Grid.h"

TEST(GridTest, ToroidalBoundaries) {
    Grid grid(5, 5);
    grid.setCell(0, 0);

    EXPECT_TRUE(grid.getCell(5, 5));    // (5,5) → (0,0)
    EXPECT_TRUE(grid.getCell(10, 10));  // (10,10) → (0,0)
    EXPECT_TRUE(grid.getCell(-5, -5));  // (-5,-5) → (0,0)

    EXPECT_FALSE(grid.getCell(-1, -1));
    EXPECT_FALSE(grid.getCell(4, 4));
    grid.setCell(4, 4);
    EXPECT_TRUE(grid.getCell(-1, -1));
    EXPECT_TRUE(grid.getCell(9, 9));
}