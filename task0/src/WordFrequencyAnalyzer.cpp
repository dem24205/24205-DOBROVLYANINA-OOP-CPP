#include "WordFrequencyAnalyzer.h"
#include <algorithm>

void WordFrequencyAnalyzer::addWord(const std::vector<std::string>& words) {
    for (const auto& word : words) {
        if (!word.empty()) {
            wordCount[word]++;
            totalWords++;
        }
    }
}

void WordFrequencyAnalyzer::updateStatistics() {
    frequencyStat.reserve(wordCount.size());
    for (const auto& [word, count] : wordCount) {
        double frequency = totalWords > 0 ?
            (static_cast<double>(count) * 100.0 / static_cast<double>(totalWords)) : 0.0;
        frequencyStat.emplace_back(word, count, frequency);
    }
    std::sort(frequencyStat.begin(), frequencyStat.end(),
        [](const WordStat& a, const WordStat& b) {
            return a.getCount() > b.getCount();
        });
}

const std::vector<WordStat> &WordFrequencyAnalyzer::getFrequencyStat() {
    updateStatistics();
    return frequencyStat;
}