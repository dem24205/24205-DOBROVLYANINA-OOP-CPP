#include "TextParser.h"
#include <cctype>
#include <string>
#include <vector>

std::vector<std::string> TextParser::parseLine(const std::string& line) {
    std::vector<std::string> words;
    std::string word;
    for (const char c : line) {
        if (std::isalnum(static_cast<unsigned char>(c))) {
            word += c;
        }
        else {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }
    return words;
}