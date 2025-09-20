#ifndef WORDFREQUENCYANALYZER_H
#define WORDFREQUENCYANALYZER_H

#include "WordStat.h"
#include <map>
#include <string>
#include <vector>

class WordFrequencyAnalyzer {
private:
    std::map<std::string, std::size_t> wordCount;
    std::size_t totalWords = 0;
    std::vector<WordStat> frequencyStat;
    void updateStatistics();

public:
    void addWord(const std::vector<std::string>& words);
    const std::vector<WordStat>& getFrequencyStat();
};

#endif