#include <gtest/gtest.h>
#include "TextParser.h"

TEST(TextParserTest, ParseBasicLine) {
    TextParser parser;
    const std::vector<std::string> result = parser.parseLine("hello world");
    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], "hello");
    EXPECT_EQ(result[1], "world");
}

TEST(TextParserTest, ParseLineWithPunctuation) {
    TextParser parser;
    const std::vector<std::string> result = parser.parseLine("Hello, world! Test.");
    EXPECT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], "Hello");
    EXPECT_EQ(result[1], "world");
    EXPECT_EQ(result[2], "Test");
}

TEST(TextParserTest, ParseLineWithApostropheAsSeparator) {
    TextParser parser;
    const std::vector<std::string> result = parser.parseLine("John's");
    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], "John");
    EXPECT_EQ(result[1], "s");
}

TEST(TextParserTest, ParseEmptyLine) {
    TextParser parser;
    const std::vector<std::string> result = parser.parseLine("");
    EXPECT_TRUE(result.empty());
}

TEST(TextParserTest, ParseOnlyDelimiters) {
    TextParser parser;
    const std::vector<std::string> result = parser.parseLine("!@#$%^&*()");
    EXPECT_TRUE(result.empty());
}

TEST(TextParserTest, ParseMixedAlphanumeric) {
    TextParser parser;
    const std::vector<std::string> result = parser.parseLine("abc123 def456");
    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], "abc123");
    EXPECT_EQ(result[1], "def456");
}