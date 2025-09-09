#include "TextParser.h"
#include <gtest/gtest.h>

TEST(TextParser, CountsWords) {
    TextParser parser;
    parser.parseLine("Hello, world! Hello 123");
    parser.parseLine("world");

    const auto& wc = parser.getWordCount();

    EXPECT_EQ(parser.getTotalWords(), 5u);
    EXPECT_EQ(wc.at("Hello"), 2u);
    EXPECT_EQ(wc.at("world"), 2u);
    EXPECT_EQ(wc.at("123"), 1);
}

TEST(TextParser, Separators) {
    TextParser parser;
    parser.parseLine("");
    parser.parseLine("! , : \n . & ?");

    EXPECT_EQ(parser.getTotalWords(), 0u);
    EXPECT_TRUE(parser.getWordCount().empty());
}