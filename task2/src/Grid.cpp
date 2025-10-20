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


void Grid::setPulsarPattern() {
    const int startX = 8;
    const int startY = 8;

    int offsets1[] = {2, 3, 4, 8, 9, 10};
    int offsets2[] = {0, 5, 7, 12};

    for (const int offset : offsets1) {
        setCell(startX + 2, startY + offset);
        setCell(startX + 7, startY + offset);
        setCell(startX + 9, startY + offset);
        setCell(startX + 14, startY + offset);
        setCell(startX + offset, startY + 2);
        setCell(startX + offset, startY + 7);
        setCell(startX + offset, startY + 9);
        setCell(startX + offset, startY + 14);
    }

    for (const int offset : offsets2) {
        setCell(startX + 4, startY + offset);
        setCell(startX + 5, startY + offset);
        setCell(startX + 10, startY + offset);
        setCell(startX + 11, startY + offset);
        setCell(startX + offset, startY + 4);
        setCell(startX + offset, startY + 5);
        setCell(startX + offset, startY + 10);
        setCell(startX + offset, startY + 11);
    }
}

void Grid::setGliderPattern() {
    const int startX = 20;
    const int startY = 20;

    setCell(startX + 1, startY + 0);
    setCell(startX + 2, startY + 1);
    setCell(startX + 0, startY + 2);
    setCell(startX + 1, startY + 2);
    setCell(startX + 2, startY + 2);
}

void Grid::setSnowflakePattern() {
    setCell(9, 0);
    setCell(25, 0);

    setCell(9, 1);
    setCell(10, 1);
    setCell(24, 1);
    setCell(25, 1);

    setCell(9, 2);
    setCell(10, 2);
    setCell(24, 2);
    setCell(25, 2);

    setCell(10, 3);
    setCell(24, 3);

    setCell(12, 8);
    setCell(13, 8);
    setCell(21, 8);
    setCell(22, 8);

    setCell(0, 9);
    setCell(1, 9);
    setCell(2, 9);
    setCell(12, 9);
    setCell(13, 9);
    setCell(14, 9);
    setCell(20, 9);
    setCell(21, 9);
    setCell(22, 9);
    setCell(32, 9);
    setCell(33, 9);
    setCell(34, 9);

    setCell(1, 10);
    setCell(2, 10);
    setCell(3, 10);
    setCell(12, 10);
    setCell(13, 10);
    setCell(14, 10);
    setCell(20, 10);
    setCell(21, 10);
    setCell(22, 10);
    setCell(31, 10);
    setCell(32, 10);
    setCell(33, 10);

    setCell(8, 12);
    setCell(26, 12);

    setCell(8, 13);
    setCell(9, 13);
    setCell(25, 13);
    setCell(26, 13);

    setCell(9, 14);
    setCell(10, 14);
    setCell(24, 14);
    setCell(25, 14);

    setCell(10, 15);
    setCell(24, 15);

    setCell(10, 19);
    setCell(24, 19);

    setCell(9, 20);
    setCell(10, 20);
    setCell(24, 20);
    setCell(25, 20);

    setCell(8, 21);
    setCell(9, 21);
    setCell(25, 21);
    setCell(26, 21);

    setCell(8, 22);
    setCell(26, 22);

    setCell(1, 24);
    setCell(2, 24);
    setCell(3, 24);
    setCell(12, 24);
    setCell(13, 24);
    setCell(14, 24);
    setCell(20, 24);
    setCell(21, 24);
    setCell(22, 24);
    setCell(31, 24);
    setCell(32, 24);
    setCell(33, 24);

    setCell(0, 25);
    setCell(1, 25);
    setCell(2, 25);
    setCell(12, 25);
    setCell(13, 25);
    setCell(14, 25);
    setCell(20, 25);
    setCell(21, 25);
    setCell(22, 25);
    setCell(32, 25);
    setCell(33, 25);
    setCell(34, 25);

    setCell(12, 26);
    setCell(13, 26);
    setCell(21, 26);
    setCell(22, 26);

    setCell(10, 31);
    setCell(24, 31);

    setCell(9, 32);
    setCell(10, 32);
    setCell(24, 32);
    setCell(25, 32);

    setCell(9, 33);
    setCell(10, 33);
    setCell(24, 33);
    setCell(25, 33);

    setCell(9, 34);
    setCell(25, 34);
}

void Grid::setBlinkerPattern() {
    const int startX = 20;
    const int startY = 20;

    setCell(startX + 1, startY + 0);
    setCell(startX + 1, startY + 1);
    setCell(startX + 1, startY + 2);
}

void Grid::setPentominoPattern() {
    const int startX = 20;
    const int startY = 20;

    setCell(startX + 1, startY + 0);
    setCell(startX + 2, startY + 0);
    setCell(startX + 0, startY + 1);
    setCell(startX + 1, startY + 1);
    setCell(startX + 1, startY + 2);
}

void Grid::setDiehardPattern() {
    const int startX = 20;
    const int startY = 20;

    setCell(startX + 0, startY + 1);
    setCell(startX + 1, startY + 1);
    setCell(startX + 1, startY + 2);
    setCell(startX + 6, startY + 0);
    setCell(startX + 5, startY + 2);
    setCell(startX + 6, startY + 2);
    setCell(startX + 7, startY + 2);
}

void Grid::set101Pattern() {
    const int startX = 8;
    const int startY = 11;

    setCell(startX + 4, startY + 0);
    setCell(startX + 5, startY + 0);
    setCell(startX + 10, startY + 0);
    setCell(startX + 11, startY + 0);

    setCell(startX + 3, startY + 1);
    setCell(startX + 5, startY + 1);
    setCell(startX + 10, startY + 1);
    setCell(startX + 12, startY + 1);

    setCell(startX + 3, startY + 2);
    setCell(startX + 12, startY + 2);

    setCell(startX + 2, startY + 3);
    setCell(startX + 4, startY + 3);
    setCell(startX + 11, startY + 3);
    setCell(startX + 13, startY + 3);

    setCell(startX + 2, startY + 4);
    setCell(startX + 4, startY + 4);
    setCell(startX + 5, startY + 4);
    setCell(startX + 6, startY + 4);
    setCell(startX + 7, startY + 4);
    setCell(startX + 8, startY + 4);
    setCell(startX + 9, startY + 4);
    setCell(startX + 10, startY + 4);
    setCell(startX + 11, startY + 4);
    setCell(startX + 13, startY + 4);

    setCell(startX + 3, startY + 5);
    setCell(startX + 5, startY + 5);
    setCell(startX + 6, startY + 5);
    setCell(startX + 7, startY + 5);
    setCell(startX + 8, startY + 5);
    setCell(startX + 9, startY + 5);
    setCell(startX + 10, startY + 5);
    setCell(startX + 12, startY + 5);

    setCell(startX + 3, startY + 6);
    setCell(startX + 5, startY + 6);
    setCell(startX + 6, startY + 6);
    setCell(startX + 7, startY + 6);
    setCell(startX + 8, startY + 6);
    setCell(startX + 9, startY + 6);
    setCell(startX + 10, startY + 6);
    setCell(startX + 12, startY + 6);

    setCell(startX + 2, startY + 7);
    setCell(startX + 4, startY + 7);
    setCell(startX + 5, startY + 7);
    setCell(startX + 6, startY + 7);
    setCell(startX + 7, startY + 7);
    setCell(startX + 8, startY + 7);
    setCell(startX + 9, startY + 7);
    setCell(startX + 10, startY + 7);
    setCell(startX + 11, startY + 7);
    setCell(startX + 13, startY + 7);

    setCell(startX + 2, startY + 8);
    setCell(startX + 4, startY + 8);
    setCell(startX + 11, startY + 8);
    setCell(startX + 13, startY + 8);

    setCell(startX + 3, startY + 9);
    setCell(startX + 12, startY + 9);

    setCell(startX + 3, startY + 10);
    setCell(startX + 5, startY + 10);
    setCell(startX + 10, startY + 10);
    setCell(startX + 12, startY + 10);

    setCell(startX + 4, startY + 11);
    setCell(startX + 5, startY + 11);
    setCell(startX + 10, startY + 11);
    setCell(startX + 11, startY + 11);
}

std::vector<std::vector<bool>> Grid::getGrid() const {
    return grid;
}