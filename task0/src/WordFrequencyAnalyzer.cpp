#include "WordFrequencyAnalyzer.h"

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <map>
#include <vector>

static std::vector<std::pair<std::string, std::size_t>>
sortWordByFrequency(const std::map<std::string, std::size_t>& wordCount) {
    std::vector<std::pair<std::string, std::size_t>>
    wc(wordCount.begin(), wordCount.end());
    std::sort(wc.begin(), wc.end(),
          [](const auto& a, const auto& b) { return a.second > b.second; });
    return wc;
}

void WordFrequencyAnalyzer::analyzeWordFrequency(const std::map<std::string,
    std::size_t>& wordCount, std::size_t totalWords) {
    const auto wc = sortWordByFrequency(wordCount);
    std::string itemStat;
    for (const auto& item : wc) {
        const double pct = totalWords ? (static_cast<double>(item.second) * 100.0 / static_cast<double>(totalWords)) : 0.0;
        std::ostringstream os;
        os << item.first << ';' << item.second << ';' << std::fixed << std::setprecision(6) << pct;
        frequencyStat.push_back(os.str());
    }
}

const std::list<std::string> &WordFrequencyAnalyzer::getFrequencyStat() const {
    return frequencyStat;
}