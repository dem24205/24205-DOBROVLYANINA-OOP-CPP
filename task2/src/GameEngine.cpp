#include "GameEngine.h"

#include <cstdlib>
#include <ctime>

void GameEngine::setRandomGridPattern() {
    std::srand(std::time(nullptr));
    const int variant = rand() % 7;
    switch(variant) {
        case 0:
            grid.setPulsarPattern();
            universeName = "Pulsar";
            break;
        case 1:
            grid.setGliderPattern();
            universeName = "Glider";
            break;
        case 2:
            grid.setSnowflakePattern();
            universeName = "Flower";
            break;
        case 3:
            grid.setBlinkerPattern();
            universeName = "Blinker";
            break;
        case 4:
            grid.setPentominoPattern();
            universeName = "R-Pentomino";
            break;
        case 5:
            grid.setDiehardPattern();
            universeName = "Diehard";
            break;
        case 6:
            grid.set101Pattern();
            universeName = "101";
            break;
        default:
            break;
    }
}

GameEngine::GameEngine(const GameConfig& config)
    : grid(config.getWidth(), config.getHeight()),
    birthRules(config.getBirthRules()),
    survivalRules(config.getSurvivalRules()),
    universeName(config.getNameUniverse()),
    version(config.getVersion()) {

    if (version == GameVersion::Batch) {
        const auto& aliveCells = config.getAliveCells();
        for (const auto [fst, snd] : aliveCells) {
            grid.setCell(fst, snd);
        }

    }
    if (version == GameVersion::Demo) {
        setRandomGridPattern();
    }
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