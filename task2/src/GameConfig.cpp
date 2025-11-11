#include "GameConfig.h"

void GameConfig::setSize(const int userWidth, const int userHeight) {
    width = userWidth;
    height = userHeight;
}

void GameConfig::setGameVersion(const GameVersion userVersion) {
    version = userVersion;
}

void GameConfig::setGameMode(const GameMode userMode) {
    mode = userMode;
}

void GameConfig::setSurvivalRules(const std::vector<int>& userSurvivalRules) {
    survivalRules = userSurvivalRules;
}

void GameConfig::setName(const std::string& userName) {
    nameUniverse = userName;
}

void GameConfig::setAliveCells(const std::vector<std::pair<int, int>>& userAliveCells) {
    aliveCells = userAliveCells;
}

void GameConfig::setBirthRules(const std::vector<int>& userBirthRules) {
    birthRules = userBirthRules;
}

void GameConfig::setOutputFile(const std::string &string) {
    outputFile = string;
}

void GameConfig::setIterations(int iters) {
    offlineIterations = iters;
}

std::string GameConfig::getOutputFile() const {
    return outputFile;
}

int GameConfig::getIterations() const {
    return offlineIterations;
}

int GameConfig::getWidth() const {
    return width;
}

int GameConfig::getHeight() const {
    return height;
}

GameMode GameConfig::getMode() const {
    return mode;
}

GameVersion GameConfig::getVersion() const {
    return version;
}

std::vector<int> GameConfig::getBirthRules() const {
    return birthRules;
}

std::vector<int> GameConfig::getSurvivalRules() const {
    return survivalRules;
}

std::string GameConfig::getNameUniverse() const {
    return nameUniverse;
}

std::vector<std::pair<int, int>> GameConfig::getAliveCells() const {
    return aliveCells;
}