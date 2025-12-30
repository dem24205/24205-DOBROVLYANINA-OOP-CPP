#include "gtest/gtest.h"
#include "CSVParser.h"
#include "TuplePrinter.h"
#include <sstream>
#include <tuple>

TEST(CSVParserTest, BasicParsing) {
    std::stringstream csv_data;
    csv_data << "1,John,25.5\n"
             << "2,Alice,30.7\n"
             << "3,Bob,22.1";
    CSVParser<int, std::string, double> parser(csv_data);
    std::vector<std::tuple<int, std::string, double>> results;
    for (const auto& row : parser) {
        results.push_back(row);
    }
    ASSERT_EQ(results.size(), 3);
    EXPECT_EQ(std::get<0>(results[0]), 1);
    EXPECT_EQ(std::get<1>(results[0]), "John");
    EXPECT_DOUBLE_EQ(std::get<2>(results[0]), 25.5);
}

TEST(CSVParserTest, SkipLines) {
    std::stringstream csv_data;
    csv_data << "header1,header2,header3\n"
             << "1,John,25\n"
             << "2,Alice,30";
    CSVParser<int, std::string, int> parser(csv_data, 1);
    std::vector<std::tuple<int, std::string, int>> results;
    for (const auto& row : parser) {
        results.push_back(row);
    }
    ASSERT_EQ(results.size(), 2);
    EXPECT_EQ(std::get<0>(results[0]), 1);
    EXPECT_EQ(std::get<1>(results[0]), "John");
    EXPECT_EQ(std::get<2>(results[0]), 25);
}

TEST(CSVParserTest, DifferentDataTypes) {
    std::stringstream csv_data;
    csv_data << "100,3.14,Hello\n"
             << "200,2.71,World";
    CSVParser<int, double, std::string> parser(csv_data);
    std::vector<std::tuple<int, double, std::string>> results;
    for (const auto& row : parser) {
        results.push_back(row);
    }
    ASSERT_EQ(results.size(), 2);
    EXPECT_EQ(std::get<0>(results[0]), 100);
    EXPECT_DOUBLE_EQ(std::get<1>(results[0]), 3.14);
    EXPECT_EQ(std::get<2>(results[0]), "Hello");
}

TEST(CSVParserTest, EmptyFile) {
    std::stringstream csv_data;
    CSVParser<int, std::string> parser(csv_data);
    int count = 0;
    for (const auto& row : parser) {
        count++;
    }
    EXPECT_EQ(count, 0);
}

TEST(CSVParserTest, SingleRow) {
    std::stringstream csv_data;
    csv_data << "42,single";
    CSVParser<int, std::string> parser(csv_data);
    int count = 0;
    for (const auto& row : parser) {
        EXPECT_EQ(std::get<0>(row), 42);
        EXPECT_EQ(std::get<1>(row), "single");
        count++;
    }
    EXPECT_EQ(count, 1);
}

TEST(CSVParserTest, ColumnCountMismatch) {
    std::stringstream csv_data;
    csv_data << "1,John\n";  // Only 2 columns, but parser expects 3
    CSVParser<int, std::string, int> parser(csv_data);
    EXPECT_THROW({
        for (const auto& row : parser) {
            // This should throw
        }
    }, CSVParserException);
}

TEST(IntegrationTest, ParseAndPrint) {
    std::stringstream csv_data;
    csv_data << "10,Apple,1.5\n"
             << "20,Banana,0.8";
    CSVParser<int, std::string, double> parser(csv_data);
    std::stringstream output;
    for (const auto& row : parser) {
        output << row << "\n";
    }
    std::string result = output.str();
    EXPECT_NE(result.find("10"), std::string::npos);
    EXPECT_NE(result.find("Apple"), std::string::npos);
    EXPECT_NE(result.find("1.5"), std::string::npos);
    EXPECT_NE(result.find("20"), std::string::npos);
}
