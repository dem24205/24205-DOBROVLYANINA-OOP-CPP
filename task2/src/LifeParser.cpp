#include "ConsoleInterface.h"
#include "LifeParser.h"

#include <set>
#include <sstream>
#include <vector>

LifeParser::LifeParser(int argc, char **argv)
    : reader(argc > 1 ? argv[1] : ""),  // инициализация в списке
      hasFile(argc > 1) {
    if (hasFile) {
        reader.open();
        if (!reader.isOpen()) {
            hasFile = false;
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
    std::vector<int> birthRules;
    const size_t start = birthConditionLine.find('B');
    const size_t end = birthConditionLine.find('/');
    if (start == std::string::npos || end == std::string::npos) {
        ConsoleInterface::printWarning("Cannot find birth rules. Default value will be used.");
        return {3}; // default B3
    }
    const std::string data = birthConditionLine.substr(start + 1, end - start);
    for (const char ch : data) {
        if (std::isdigit(ch)) {
            birthRules.push_back(ch - '0');
        }
    }
    if (birthRules.empty()) {
        ConsoleInterface::printWarning("Birth rules are empty. Default value will be used.");
        return {3};
    }
    for (const int rule : birthRules) {
        if (rule < 0 || rule > 8) {
            ConsoleInterface::printWarning("Birth rule " + std::to_string(rule) + " is invalid. Rules must be 0-8.");
            ConsoleInterface::printWarning("Default value will be used.");
            return {3};
        }
    }
    return birthRules;
}

std::vector<int> LifeParser::parseSurvivalCondition(const std::string& survivalConditionLine) {
    std::vector<int> survivalRules;
    const size_t start = survivalConditionLine.find('S');
    if (start == std::string::npos) {
        ConsoleInterface::printWarning("Cannot find survival rule. Default value will be used.");
        return {2, 3}; // default S23
    }
    const std::string data = survivalConditionLine.substr(start + 1);
    for (const char ch : data) {
        if (std::isdigit(ch)) {
            survivalRules.push_back(ch - '0');
        }
    }
    if (survivalRules.empty()) {
        ConsoleInterface::printWarning("Survival rules are empty. Default value will be used.");
        return {2, 3};
    }
    for (const int rule : survivalRules) {
        if (rule < 0 || rule > 8) {
            ConsoleInterface::printWarning("Survival rule " + std::to_string(rule) + " is invalid. Rules must be 0-8.");
            ConsoleInterface::printWarning("Default value will be used.");
            return {2, 3};
        }
    }
    return survivalRules;
}

std::pair<int, int> LifeParser::parseGridSize(const std::string& gridSizeLine) {
    size_t start = gridSizeLine.find('C') + 1;
    const size_t end = gridSizeLine.find('/');
    if (start == std::string::npos || end == std::string::npos) {
        ConsoleInterface::printWarning("Cannot find size of grid. Default value will be used.");
        return {35, 35}; // default
    }
    if (start >= end) {
        ConsoleInterface::printWarning("Invalid column size. Default value will be used.");
        return {35, 35};
    }
    const std::string colStr = gridSizeLine.substr(start, end - start);
    for (const char c : colStr) {
        if (!std::isdigit(c)) {
            ConsoleInterface::printWarning("Invalid column size format. Default value will be used.");
            return {35, 35};
        }
    }
    start = gridSizeLine.find('R') + 1;
    if (start == std::string::npos) {
        ConsoleInterface::printWarning("Cannot find row size. Default value will be used.");
        return {35, 35}; // default
    }
    const std::string rowStr = gridSizeLine.substr(start);
    for (const char c : rowStr) {
        if (!std::isdigit(c)) {
            ConsoleInterface::printWarning("Invalid row size format. Default value will be used.");
            return {35, 35};
        }
    }
    const int column = std::stoi(colStr);
    const int row = std::stoi(rowStr);
    if (column <= 0 || row <= 0) {
        ConsoleInterface::printWarning("Grid size must be positive. Default value will be used.");
        return {35, 35};
    }
    return {column, row};
}

std::vector<std::pair<int, int>> LifeParser::parseAliveCells() {
    std::vector<std::pair<int, int>> cells;
    std::set<std::pair<int, int>> uniqueCells;
    while (!reader.isEOF()) {
        std::string line = reader.getLine();
        if (line.empty()) continue;
        std::istringstream iss(line);
        int x, y;
        if (iss >> x >> y) {
            auto cell = std::make_pair(x, y);
            if (uniqueCells.count(cell)) {
                ConsoleInterface::printWarning("Duplicate cell coordinates (" +
                    std::to_string(x) + "," + std::to_string(y) + ") ignored.");
            } else {
                cells.push_back(cell);
                uniqueCells.insert(cell);
            }
        } else {
            ConsoleInterface::printWarning("Invalid cell format in line: " + line);
        }
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

void LifeParser::handleParsing(GameConfig &config, int argc, char **argv) {
    if (!hasFile) {
        return;
    }

    bool hasIterations = false;
    bool hasOutput = false;

    parseCommandLineArgs(config, argc, argv, hasIterations, hasOutput);
    std::string format = reader.getLine();
    if (!isRightFormat(format)) {
        ConsoleInterface::printWarning("File is not in Life 1.06 format. Using demo configuration.");
        if (hasIterations || hasOutput) {
            config.setGameMode(GameMode::Offline);
        } else {
            config.setGameMode(GameMode::Online);
        }
        return;
    }

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
        ConsoleInterface::printWarning("No alive cells found in file. Using random universe");
        config.setGameVersion(GameVersion::Demo);
    } else {
        config.setAliveCells(cells);
        config.setGameVersion(GameVersion::Batch);
    }

    if (hasIterations || hasOutput) {
        config.setGameMode(GameMode::Offline);
    } else {
        config.setGameMode(GameMode::Online);
    }
}