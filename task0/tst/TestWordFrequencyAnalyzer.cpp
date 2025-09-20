#include "WordFrequencyAnalyzer.h"
#include <gtest/gtest.h>

TEST(WordFrequencyAnalyzerTest, AddSingleWord) {
    WordFrequencyAnalyzer analyzer;
    analyzer.addWord({"hello"});
    const auto stats = analyzer.getFrequencyStat();
    EXPECT_EQ(stats.size(), 1);
    EXPECT_EQ(stats[0].getWord(), "hello");
    EXPECT_EQ(stats[0].getCount(), 1);
    EXPECT_DOUBLE_EQ(stats[0].getFrequency(), 100.0);
}

TEST(WordFrequencyAnalyzerTest, AddMultipleWords) {
    WordFrequencyAnalyzer analyzer;
    analyzer.addWord({"hello", "world", "hello"});

    const auto stats = analyzer.getFrequencyStat();
    EXPECT_EQ(stats.size(), 2);

    EXPECT_EQ(stats[0].getWord(), "hello");
    EXPECT_EQ(stats[0].getCount(), 2);
    EXPECT_DOUBLE_EQ(stats[0].getFrequency(), (2.0 / 3.0) * 100.0);

    EXPECT_EQ(stats[1].getWord(), "world");
    EXPECT_EQ(stats[1].getCount(), 1);
    EXPECT_DOUBLE_EQ(stats[1].getFrequency(), (1.0 / 3.0) * 100.0);
}

TEST(WordFrequencyAnalyzerTest, AddEmptyWord) {
    WordFrequencyAnalyzer analyzer;
    analyzer.addWord({"hello", "", "world"});
    const auto stats = analyzer.getFrequencyStat();
    EXPECT_EQ(stats.size(), 2);
}

TEST(WordFrequencyAnalyzerTest, SortByFrequencyDescending) {
    WordFrequencyAnalyzer analyzer;
    analyzer.addWord({"a", "b", "c", "a", "b", "a"});
    auto const stats = analyzer.getFrequencyStat();
    EXPECT_EQ(stats.size(), 3);
    EXPECT_EQ(stats[0].getWord(), "a");
    EXPECT_EQ(stats[0].getCount(), 3);

    EXPECT_EQ(stats[1].getWord(), "b");
    EXPECT_EQ(stats[1].getCount(), 2);

    EXPECT_EQ(stats[2].getWord(), "c");
    EXPECT_EQ(stats[2].getCount(), 1);
}

TEST(WordFrequencyAnalyzerTest, MultipleAddCalls) {
    WordFrequencyAnalyzer analyzer;
    analyzer.addWord({"hello", "world"});
    analyzer.addWord({"hello", "test"});
    auto const stats = analyzer.getFrequencyStat();
    EXPECT_EQ(stats.size(), 3);
    EXPECT_EQ(stats[0].getWord(), "hello");
    EXPECT_EQ(stats[0].getCount(), 2);
}

TEST(WordFrequencyAnalyzerTest, ZeroTotalWords) {
    WordFrequencyAnalyzer analyzer;
    auto const stats = analyzer.getFrequencyStat();
    EXPECT_TRUE(stats.empty());
}