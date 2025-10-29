#include "GameEngine.h"
#include <random>
GameEngine::GameEngine(const GameConfig& config)
    : grid(config.getWidth(), config.getHeight()),
    birthRules(config.getBirthRules()),
    survivalRules(config.getSurvivalRules()),
    universeName(config.getNameUniverse()),
    version(config.getVersion()) {

    if (version == GameVersion::Batch) {
        const auto& aliveCells = config.getAliveCells();
        if (aliveCells.empty()) {
            setGliderPattern();
        }
        else {
            for (const auto [fst, snd] : aliveCells) {
                grid.setCell(fst, snd);
            }
        }
        return;
    }
    if (version == GameVersion::Demo) {
        setRandomGridPattern();
    }
}

void GameEngine::setRandomGridPattern() {
    std::random_device rd;
    std::uniform_int_distribution<int> dist(0, 6);
    const int variant = dist(rd);
    switch(variant) {
        case 0:
            setPulsarPattern();
            universeName = "Pulsar";
            break;
        case 1:
            setGliderPattern();
            universeName = "Glider";
            break;
        case 2:
            setSnowflakePattern();
            universeName = "Snowflake";
            break;
        case 3:
            setBlinkerPattern();
            universeName = "Blinker";
            break;
        case 4:
            setPentominoPattern();
            universeName = "R-Pentomino";
            break;
        case 5:
            setDiehardPattern();
            universeName = "Diehard";
            break;
        case 6:
            set101Pattern();
            universeName = "101";
            break;
        default:
            break;
    }
}

void GameEngine::setGliderPattern() {
    constexpr int startX = 20;
    constexpr int startY = 20;
    grid.setCell(startX + 1, startY + 0);
    grid.setCell(startX + 2, startY + 1);
    grid.setCell(startX + 0, startY + 2);
    grid.setCell(startX + 1, startY + 2);
    grid.setCell(startX + 2, startY + 2);
}

void GameEngine::setDiehardPattern() {
    constexpr int startX = 20;
    constexpr int startY = 20;
    grid.setCell(startX + 0, startY + 1);
    grid.setCell(startX + 1, startY + 1);
    grid.setCell(startX + 1, startY + 2);
    grid.setCell(startX + 6, startY + 0);
    grid.setCell(startX + 5, startY + 2);
    grid.setCell(startX + 6, startY + 2);
    grid.setCell(startX + 7, startY + 2);
}

void GameEngine::setBlinkerPattern() {
    constexpr int startX = 20;
    constexpr int startY = 20;
    grid.setCell(startX + 1, startY + 0);
    grid.setCell(startX + 1, startY + 1);
    grid.setCell(startX + 1, startY + 2);
}

void GameEngine::setPentominoPattern() {
    constexpr int startX = 20;
    constexpr int startY = 20;
    grid.setCell(startX + 1, startY + 0);
    grid.setCell(startX + 2, startY + 0);
    grid.setCell(startX + 0, startY + 1);
    grid.setCell(startX + 1, startY + 1);
    grid.setCell(startX + 1, startY + 2);
}

void GameEngine::setPulsarPattern() {
    constexpr int startX = 8;
    constexpr int startY = 8;
    int offsets1[] = {2, 3, 4, 8, 9, 10};
    int offsets2[] = {0, 5, 7, 12};
    for (const int offset : offsets1) {
        grid.setCell(startX + 2, startY + offset);
        grid.setCell(startX + 7, startY + offset);
        grid.setCell(startX + 9, startY + offset);
        grid.setCell(startX + 14, startY + offset);
        grid.setCell(startX + offset, startY + 2);
        grid.setCell(startX + offset, startY + 7);
        grid.setCell(startX + offset, startY + 9);
        grid.setCell(startX + offset, startY + 14);
    }
    for (const int offset : offsets2) {
        grid.setCell(startX + 4, startY + offset);
        grid.setCell(startX + 5, startY + offset);
        grid.setCell(startX + 10, startY + offset);
        grid.setCell(startX + 11, startY + offset);
        grid.setCell(startX + offset, startY + 4);
        grid.setCell(startX + offset, startY + 5);
        grid.setCell(startX + offset, startY + 10);
        grid.setCell(startX + offset, startY + 11);
    }
}

void GameEngine::set101Pattern() {
    constexpr int startX = 8;
    constexpr int startY = 11;
    grid.setCell(startX + 4, startY + 0);
    grid.setCell(startX + 5, startY + 0);
    grid.setCell(startX + 10, startY + 0);
    grid.setCell(startX + 11, startY + 0);
    grid.setCell(startX + 3, startY + 1);
    grid.setCell(startX + 5, startY + 1);
    grid.setCell(startX + 10, startY + 1);
    grid.setCell(startX + 12, startY + 1);
    grid.setCell(startX + 3, startY + 2);
    grid.setCell(startX + 12, startY + 2);
    grid.setCell(startX + 2, startY + 3);
    grid.setCell(startX + 4, startY + 3);
    grid.setCell(startX + 11, startY + 3);
    grid.setCell(startX + 13, startY + 3);
    grid.setCell(startX + 2, startY + 4);
    grid.setCell(startX + 4, startY + 4);
    grid.setCell(startX + 5, startY + 4);
    grid.setCell(startX + 6, startY + 4);
    grid.setCell(startX + 7, startY + 4);
    grid.setCell(startX + 8, startY + 4);
    grid.setCell(startX + 9, startY + 4);
    grid.setCell(startX + 10, startY + 4);
    grid.setCell(startX + 11, startY + 4);
    grid.setCell(startX + 13, startY + 4);
    grid.setCell(startX + 3, startY + 5);
    grid.setCell(startX + 5, startY + 5);
    grid.setCell(startX + 6, startY + 5);
    grid.setCell(startX + 7, startY + 5);
    grid.setCell(startX + 8, startY + 5);
    grid.setCell(startX + 9, startY + 5);
    grid.setCell(startX + 10, startY + 5);
    grid.setCell(startX + 12, startY + 5);
    grid.setCell(startX + 3, startY + 6);
    grid.setCell(startX + 5, startY + 6);
    grid.setCell(startX + 6, startY + 6);
    grid.setCell(startX + 7, startY + 6);
    grid.setCell(startX + 8, startY + 6);
    grid.setCell(startX + 9, startY + 6);
    grid.setCell(startX + 10, startY + 6);
    grid.setCell(startX + 12, startY + 6);
    grid.setCell(startX + 2, startY + 7);
    grid.setCell(startX + 4, startY + 7);
    grid.setCell(startX + 5, startY + 7);
    grid.setCell(startX + 6, startY + 7);
    grid.setCell(startX + 7, startY + 7);
    grid.setCell(startX + 8, startY + 7);
    grid.setCell(startX + 9, startY + 7);
    grid.setCell(startX + 10, startY + 7);
    grid.setCell(startX + 11, startY + 7);
    grid.setCell(startX + 13, startY + 7);
    grid.setCell(startX + 2, startY + 8);
    grid.setCell(startX + 4, startY + 8);
    grid.setCell(startX + 11, startY + 8);
    grid.setCell(startX + 13, startY + 8);
    grid.setCell(startX + 3, startY + 9);
    grid.setCell(startX + 12, startY + 9);
    grid.setCell(startX + 3, startY + 10);
    grid.setCell(startX + 5, startY + 10);
    grid.setCell(startX + 10, startY + 10);
    grid.setCell(startX + 12, startY + 10);
    grid.setCell(startX + 4, startY + 11);
    grid.setCell(startX + 5, startY + 11);
    grid.setCell(startX + 10, startY + 11);
    grid.setCell(startX + 11, startY + 11);
}

void GameEngine::setSnowflakePattern() {
    grid.setCell(9, 2); grid.setCell(10, 2);
    grid.setCell(24, 2); grid.setCell(25, 2);
    grid.setCell(10, 3); grid.setCell(24, 3);
    grid.setCell(12, 8); grid.setCell(13, 8);
    grid.setCell(21, 8); grid.setCell(22, 8);
    grid.setCell(0, 9); grid.setCell(1, 9); grid.setCell(2, 9);
    grid.setCell(12, 9); grid.setCell(13, 9); grid.setCell(14, 9);
    grid.setCell(20, 9); grid.setCell(21, 9); grid.setCell(22, 9);
    grid.setCell(32, 9); grid.setCell(33, 9); grid.setCell(34, 9);
    grid.setCell(1, 10); grid.setCell(2, 10); grid.setCell(3, 10);
    grid.setCell(12, 10); grid.setCell(13, 10); grid.setCell(14, 10);
    grid.setCell(20, 10); grid.setCell(21, 10); grid.setCell(22, 10);
    grid.setCell(31, 10); grid.setCell(32, 10); grid.setCell(33, 10);
    grid.setCell(8, 12);
    grid.setCell(26, 12);
    grid.setCell(8, 13); grid.setCell(9, 13);
    grid.setCell(25, 13); grid.setCell(26, 13);
    grid.setCell(9, 14); grid.setCell(10, 14);
    grid.setCell(24, 14); grid.setCell(25, 14);
    grid.setCell(10, 15);
    grid.setCell(24, 15);
    grid.setCell(10, 19);
    grid.setCell(24, 19);
    grid.setCell(9, 20); grid.setCell(10, 20);
    grid.setCell(24, 20); grid.setCell(25, 20);
    grid.setCell(8, 21); grid.setCell(9, 21);
    grid.setCell(25, 21); grid.setCell(26, 21);
    grid.setCell(8, 22);
    grid.setCell(26, 22);
    grid.setCell(1, 24); grid.setCell(2, 24); grid.setCell(3, 24);
    grid.setCell(12, 24); grid.setCell(13, 24); grid.setCell(14, 24);
    grid.setCell(20, 24); grid.setCell(21, 24); grid.setCell(22, 24);
    grid.setCell(31, 24); grid.setCell(32, 24); grid.setCell(33, 24);
    grid.setCell(0, 25); grid.setCell(1, 25); grid.setCell(2, 25);
    grid.setCell(12, 25); grid.setCell(13, 25); grid.setCell(14, 25);
    grid.setCell(20, 25); grid.setCell(21, 25); grid.setCell(22, 25);
    grid.setCell(32, 25); grid.setCell(33, 25); grid.setCell(34, 25);
    grid.setCell(12, 26); grid.setCell(13, 26);
    grid.setCell(21, 26); grid.setCell(22, 26);
    grid.setCell(10, 31);
    grid.setCell(24, 31);
    grid.setCell(9, 32); grid.setCell(10, 32);
    grid.setCell(24, 32); grid.setCell(25, 32);
    grid.setCell(9, 33); grid.setCell(10, 33);
    grid.setCell(24, 33); grid.setCell(25, 33);
    grid.setCell(9, 34);
    grid.setCell(25, 34);
}


int GameEngine::countNeighbors(const int x, const int y) const {
    int count = 0;
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) {
                continue;
            }
            if (grid.getCell(x + dx, y + dy)) {
                count++;
            }
        }
    }
    return count;
}

void GameEngine::countNextGeneration() {
    Grid newGrid(grid.getWidth(), grid.getHeight());
    const int height = grid.getHeight();
    const int width = grid.getWidth();
    for (int y = 0 ; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const bool current = grid.getCell(x, y);
            const int count = countNeighbors(x, y);
            if (!current) {
                for (const auto rule : birthRules) {
                    if (count == rule) {
                        newGrid.setCell(x, y);
                        break;
                    }
                }
            }
            else {
                bool isSurvived = false;
                for (const auto rule : survivalRules) {
                    if (count == rule) {
                        isSurvived = true;
                        break;
                    }
                }
                newGrid.setCell(x, y, isSurvived);
            }
        }
    }
    grid = newGrid;
}

void GameEngine::countGenerations(const int gens) {
    for (int i = 0; i < gens; ++i) {
        countNextGeneration();
    }
}

std::vector<std::vector<bool>> GameEngine::getGrid() const {
    return grid.getGrid();
}

std::vector<int> GameEngine::getBirthRules() const {
    return birthRules;
}

std::vector<int> GameEngine::getSurvivalRules() const {
    return survivalRules;
}

std::string GameEngine::getUniverseName() const {
    return universeName;
}

int GameEngine::getWidth() const {
    return grid.getWidth();
}

int GameEngine::getHeight() const {
    return grid.getHeight();
}