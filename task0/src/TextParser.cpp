#include "TextParser.h"
#include <cctype>

void TextParser::parseLine(const std::string& line) {
    std::string word;
    for (const char c : line) {
        if (std::isalnum(static_cast<unsigned char>(c))) {
            word += c;
        }
        else {
            if (!word.empty()) {
                wordCount[word]++;
                totalWords++;
                word.clear();
            }
        }
    }
    if (!word.empty()) {
        wordCount[word]++;
        totalWords++;
    }
}

const std::map<std::string, std::size_t> &TextParser::getWordCount() const {
    return wordCount;
}

std::size_t TextParser::getTotalWords() const {
    return totalWords;
}