#include <gtest/gtest.h>
#include <sstream>
#include "CSVParser.h"

//config: column delim
TEST(ImprovedCSVParserTest, CustomDelimiters) {
    std::stringstream ss;
    ss << "1;John;25.5\n";
    ss << "2;Jane;30.0";
    CSVParserConfig config(';', '\n', '"');
    CSVParser<int, std::string, double> parser(ss, config);
    auto it = parser.begin();
    auto row = *it;
    EXPECT_EQ(std::get<0>(row), 1);
    EXPECT_EQ(std::get<1>(row), "John");
    EXPECT_DOUBLE_EQ(std::get<2>(row), 25.5);
}

//config: row delim
TEST(ImprovedCSVParserTest, CustomRowDelimiter) {
    std::stringstream ss;
    ss << "1,John,25|2,Jane,30|3,Bob,35";
    CSVParserConfig config(',', '|', '"');
    CSVParser<int, std::string, int> parser(ss, config);
    std::vector<std::tuple<int, std::string, int>> results;
    for (auto& row : parser) {
        results.push_back(row);
    }
    EXPECT_EQ(results.size(), 3);
    EXPECT_EQ(std::get<0>(results[0]), 1);
    EXPECT_EQ(std::get<1>(results[1]), "Jane");
    EXPECT_EQ(std::get<2>(results[2]), 35);
}

//screening: unexpected symbols after escape
TEST(ImprovedCSVParserTest, UnexpectedCharsAfterQuote) {
    std::stringstream ss;
    ss << "1,\"John\"Smith,25\n";
    CSVParser<int, std::string, int> parser(ss);
    EXPECT_THROW({
        auto it = parser.begin();
        *it;
    }, CSVParserException);
}

//screening: simple valid test
TEST(ImprovedCSVParserTest, DelimiterInsideQuotes) {
    std::stringstream ss;
    ss << "1,\"Doe, John\",25\n";
    ss << "2,\"Smith;Jane\",30";
    CSVParser<int, std::string, int> parser(ss);
    auto it = parser.begin();
    auto row1 = *it;
    EXPECT_EQ(std::get<1>(row1), "Doe, John");
    ++it;
    auto row2 = *it;
    EXPECT_EQ(std::get<1>(row2), "Smith;Jane");
}

//screening: double quotes
TEST(ImprovedCSVParserTest, DoubleQuotesInsideField) {
    std::stringstream ss;
    ss << "1,\"John \"\"The Rock\"\"\",25\n";
    CSVParser<int, std::string, int> parser(ss);
    auto it = parser.begin();
    auto row = *it;
    EXPECT_EQ(std::get<1>(row), "John \"The Rock\"");
}

//screening: unclosed quote
TEST(ImprovedCSVParserTest, UnclosedQuoteError) {
    std::stringstream ss;
    ss << "1,\"Unclosed quote,2\n";
    CSVParser<int, std::string> parser(ss);
    EXPECT_THROW({
        auto it = parser.begin();
        *it;
    }, CSVParserException);
}

//offset: skip header file
TEST(ImprovedCSVParserTest, SkipRows) {
    std::stringstream ss;
    ss << "Header1,Header2,Header3\n";
    ss << "1,John,25\n";
    ss << "2,Jane,30\n";
    CSVParser<int, std::string, int> parser(ss, 1);
    auto it = parser.begin();
    auto row = *it;
    EXPECT_EQ(std::get<0>(row), 1);
    EXPECT_EQ(std::get<1>(row), "John");
}

//exception: mismatch
TEST(ImprovedCSVParserTest, ColumnCountMismatch) {
    std::stringstream ss;
    ss << "1,John,25,Extra\n";
    ss << "2,Jane";
    CSVParser<int, std::string, int> parser(ss);
    EXPECT_THROW({
        auto it = parser.begin();
        ++it;
    }, CSVParserException);
}
