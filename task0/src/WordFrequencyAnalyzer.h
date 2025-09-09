#ifndef WORDFREQUENCYANALYZER_H
#define WORDFREQUENCYANALYZER_H

#include <list>
#include <map>
#include <string>

class WordFrequencyAnalyzer {
private:
    std::list<std::string> frequencyStat;
public:
    void analyzeWordFrequency(const std::map<std::string, std::size_t>& wordCount, std::size_t totalWords);
    const std::list<std::string>& getFrequencyStat() const;
};

#endif