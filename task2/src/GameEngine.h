#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "GameConfig.h"
#include "Grid.h"

#include <vector>

class GameEngine {
private:
    Grid grid;
    std::vector<int> birthRules;
    std::vector<int> survivalRules;
    std::string universeName;
    GameVersion version;
    void setSnowflakePattern();
    void setPulsarPattern();
    void setGliderPattern();
    void setBlinkerPattern();
    void setPentominoPattern();
    void setDiehardPattern();
    void set101Pattern();
    void setRandomGridPattern();
public:
    explicit GameEngine(const GameConfig& config);
    int countNeighbors(int x, int y) const;
    void countNextGeneration();
    void countGenerations(int gens);
    std::vector<std::vector<bool>> getGrid() const;
    std::vector<int> getBirthRules() const;
    std::vector<int> getSurvivalRules() const;
    std::string getUniverseName() const;
    int getWidth() const;
    int getHeight() const;
};

#endif