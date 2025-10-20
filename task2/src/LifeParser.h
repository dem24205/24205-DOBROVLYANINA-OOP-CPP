#ifndef LIFEPARSER_H
#define LIFEPARSER_H

#include "GameConfig.h"
#include "LifeFileReader.h"

#include <string>
#include <vector>

class LifeParser {
private:
    LifeFileReader reader;
    bool hasFile = false;
public:
    explicit LifeParser(int argc, char** argv);
    void parseCommandLineArgs(GameConfig& config, int argc, char** argv, bool& hasIterations, bool& hasOutput);
    std::string parseUniverseName(const std::string& nameLine);
    std::vector<int> parseBirthCondition(const std::string& birthConditionLine);
    std::vector<int> parseSurvivalCondition(const std::string& survivalConditionLine);
    std::pair<int, int> parseGridSize(const std::string& gridSizeLine);
    std::vector<std::pair<int, int>> parseAliveCells();
    void handleParsing(GameConfig& config, int argc, char **argv);
};

#endif
