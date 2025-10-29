#include "Grid.h"

Grid::Grid(const int newWidth, const int newHeight) : width(newWidth), height(newHeight) {
    const std::vector<std::vector<bool>>newGrid(newHeight, std::vector<bool>(newWidth, false));
    grid = newGrid;
}

void Grid::setCell(int x, int y, const bool value) {
    x = (x + width) % width;
    y = (y + height) % height;
    grid[y][x] = value;
}

bool Grid::getCell (int x, int y) const {
    x = (x + width) % width;
    y = (y + height) % height;
    return grid[y][x];
}

int Grid::getWidth() const {
    return width;
}

int Grid::getHeight() const {
    return height;
}

std::vector<std::vector<bool>> Grid::getGrid() const {
    return grid;
}