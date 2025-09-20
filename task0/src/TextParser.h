#ifndef TEXTPARSER_H
#define TEXTPARSER_H

#include <string>
#include <vector>

class TextParser {
public:
    std::vector<std::string> parseLine(const std::string& line);
};

#endif