#ifndef TEXTPARSER_H
#define TEXTPARSER_H

#include <map>
#include <string>

class TextParser {
private:
    std::map<std::string, std::size_t> wordCount;
    std::size_t totalWords = 0;
public:
    void parseLine(const std::string& line);
    const std::map<std::string, std::size_t>& getWordCount() const;
    std::size_t getTotalWords() const;
};

#endif