#include "ConsoleInterface.h"
#include "LifeParser.h"
#include <set>
#include <sstream>
#include <vector>
#include <iostream>

LifeParser::LifeParser(const std::string &filename): reader(filename) {
    if (!filename.empty()) {
        reader.open();
        if (!reader.isOpen()) {
            hasFile = false;
        }
        else {
            hasFile = true;
        }
    }
}

static bool isRightFormat(const std::string& format) {
    return (format == "#Life 1.06");
}

std::string LifeParser::parseUniverseName(const std::string& nameLine) {
    const size_t start = nameLine.find("#N ");
    if (start == std::string::npos) {
        ConsoleInterface::printWarning("Cannot find name of universe.");
        return "";
    }
    return nameLine.substr(start + 3);
}

std::vector<int> LifeParser::parseBirthCondition(const std::string& birthConditionLine) {
    const size_t start = birthConditionLine.find('B');
    const size_t end = birthConditionLine.find('/');
    if (start == std::string::npos || end == std::string::npos) {
        ConsoleInterface::printWarning("Invalid birth rules format. Default B3 will be used.");
        return {3};
    }
    std::vector<int> birthRules;
    for (const char ch : birthConditionLine.substr(start + 1, end - start - 1)) {
        if (std::isdigit(ch)) {
            const int rule = ch - '0';
            if (rule >= 0 && rule <= 8) {
                birthRules.push_back(rule);
            }
        }
    }
    if (birthRules.empty()) {
        ConsoleInterface::printWarning("No valid birth rules found. Default B3 will be used.");
        return {3};
    }
    return birthRules;
}

std::vector<int> LifeParser::parseSurvivalCondition(const std::string& survivalConditionLine) {
    const size_t start = survivalConditionLine.find('S');
    if (start == std::string::npos) {
        ConsoleInterface::printWarning("Invalid survival rules format. Default S23 will be used.");
        return {2, 3};
    }
    std::vector<int> survivalRules;
    for (const char ch : survivalConditionLine.substr(start + 1)) {
        if (std::isdigit(ch)) {
            const int rule = ch - '0';
            if (rule >= 0 && rule <= 8) {
                survivalRules.push_back(rule);
            }
        }
    }
    if (survivalRules.empty()) {
        ConsoleInterface::printWarning("No valid survival rules found. Default S23 will be used.");
        return {2, 3};
    }
    return survivalRules;
}

std::pair<int, int> LifeParser::parseGridSize(const std::string& gridSizeLine) {
    size_t cPos = gridSizeLine.find('C');
    size_t slashPos = gridSizeLine.find('/');
    size_t rPos = gridSizeLine.find('R');
    if (cPos == std::string::npos || slashPos == std::string::npos || rPos == std::string::npos) {
        ConsoleInterface::printWarning("Invalid grid size format. Default 35x35 will be used.");
        return {35, 35};
    }

    const std::string colStr = gridSizeLine.substr(cPos + 1, slashPos - cPos - 1);
    const std::string rowStr = gridSizeLine.substr(rPos + 1);

    try {
        int column = std::stoi(colStr);
        int row = std::stoi(rowStr);
        if (column > 0 && row > 0) {
            return {column, row};
        }
    }
    catch (...) {
    }
    ConsoleInterface::printWarning("Invalid grid size. Default 35x35 will be used.");
    return {35, 35};
}

std::vector<std::pair<int, int>> LifeParser::parseAliveCells() {
    std::vector<std::pair<int, int>> cells;
    std::set<std::pair<int, int>> uniqueCells;
    while (!reader.isEOF()) {
        std::string line = reader.getLine();
        if (line.empty()) continue;
        std::istringstream iss(line);
        int x, y;
        if (!(iss >> x >> y)) {
            ConsoleInterface::printWarning("Invalid cell format: " + line);
            continue;
        }
        auto cell = std::make_pair(x, y);
        if (!uniqueCells.insert(cell).second) {
            ConsoleInterface::printWarning("Duplicate cell (" + std::to_string(x) + "," + std::to_string(y) + ") ignored.");
            continue;
        }
        cells.push_back(cell);
    }
    return cells;
}

void LifeParser::parseCommandLineArgs(GameConfig& config, const int argc, char** argv, bool& hasIterations, bool& hasOutput) {
    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.find("--iterations=") == 0 || arg.find("-i=") == 0) {
            hasIterations = true;
            const size_t pos = arg.find('=');
            if (pos != std::string::npos) {
                try {
                    const int iters = std::stoi(arg.substr(pos + 1));
                    config.setIterations(iters);
                } catch (...) {
                    ConsoleInterface::printWarning("Invalid iteration count. Default will be used.");
                }
            }
        }
        if (arg.find("--output=") == 0 || arg.find("-o=") == 0) {
            hasOutput = true;
            const size_t pos = arg.find('=');
            if (pos != std::string::npos) {
                config.setOutputFile(arg.substr(pos + 1));
            }
        }
    }
}

GameConfig LifeParser::handleParsing(const int argc, char **argv) {
    GameConfig config;
    if (!hasFile) {
        return config;
    }
    bool hasIterations = false;
    bool hasOutput = false;
    parseCommandLineArgs(config, argc, argv, hasIterations, hasOutput);
    config.setGameMode(hasIterations || hasOutput ? GameMode::Offline : GameMode::Online);
    std::string format = reader.getLine();
    if (!isRightFormat(format)) {
        ConsoleInterface::printWarning("File is not in Life 1.06 format. Demo universe will be used.");
        return config;
    }
    config.setGameVersion(GameVersion::Batch);
    const std::string universeNameLine = reader.getLine();
    const std::string rulesLine = reader.getLine();
    const std::string gridSizeLine = reader.getLine();

    const std::string name = parseUniverseName(universeNameLine);
    const std::vector<int> birth = parseBirthCondition(rulesLine);
    const std::vector<int> survival = parseSurvivalCondition(rulesLine);
    auto [w, h] = parseGridSize(gridSizeLine);

    config.setName(name);
    config.setBirthRules(birth);
    config.setSurvivalRules(survival);
    config.setSize(w, h);

    std::vector<std::pair<int, int>> cells = parseAliveCells();
    if (cells.empty()) {
        ConsoleInterface::printWarning("No alive cells found in file. Simple Glider universe will be used.");
    }
    else {
        config.setAliveCells(cells);
    }
    return config;
}