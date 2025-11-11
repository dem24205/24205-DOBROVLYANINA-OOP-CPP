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
    explicit LifeParser(const std::string& filename);
    static void parseCommandLineArgs(GameConfig& config, int argc, char** argv, bool& hasIterations, bool& hasOutput);
    static std::string parseUniverseName(const std::string& nameLine);
    static std::vector<int> parseBirthCondition(const std::string& birthConditionLine);
    static std::vector<int> parseSurvivalCondition(const std::string& survivalConditionLine);
    static std::pair<int, int> parseGridSize(const std::string& gridSizeLine);
    std::vector<std::pair<int, int>> parseAliveCells();
    GameConfig handleParsing(int argc, char **argv);
};

#endif
