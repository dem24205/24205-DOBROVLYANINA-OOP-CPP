#include "WordFrequencyAnalyzer.h"
#include <gtest/gtest.h>

TEST(WordFrequencyAnalyzer, ProduceSortedStatByFrequency) {
    std::map<std::string, std::size_t> wc{
                {"Hello", 5},
                {"world", 6},
                {"123", 10}
    };
    std::size_t total = 20;
    WordFrequencyAnalyzer analyzer;
    analyzer.analyzeWordFrequency(wc, total);
    const std::list<std::string>& freqStat = analyzer.getFrequencyStat();

    ASSERT_EQ(freqStat.size(), 3u);

    std::list<std::string> expected = {
        "123;10;50.000000",
        "world;6;30.000000",
        "Hello;5;25.000000"
    };

    EXPECT_EQ(freqStat, expected);
}

TEST(WordFrequencyAnalyzer, HandlesEmptyInput) {
    WordFrequencyAnalyzer analyzer;

    std::map<std::string, std::size_t> wc{};
    std::size_t total = 0;

    analyzer.analyzeWordFrequency(wc, total);
    const auto& rows = analyzer.getFrequencyStat();
    EXPECT_TRUE(rows.empty());
}