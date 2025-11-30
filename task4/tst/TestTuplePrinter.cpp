#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include "TuplePrinter.h"

class TuplePrinterTest : public ::testing::Test {
protected:
    std::stringstream ss;
};

//tuple with multiple types
TEST_F(TuplePrinterTest, BasicTuple) {
    std::tuple<int, double, std::string> t{42, 3.14, "hello"};
    ss << t;
    EXPECT_EQ(ss.str(), "( 42 3.14 hello )");
}

//single element tuple
TEST_F(TuplePrinterTest, SingleElement) {
    std::tuple<std::string> t{"alone"};
    ss << t;
    EXPECT_EQ(ss.str(), "( alone )");
}

//empty tuple
TEST_F(TuplePrinterTest, EmptyTuple) {
    std::tuple<> t;
    ss << t;
    EXPECT_EQ(ss.str(), "( )");
}

//large tuple
TEST_F(TuplePrinterTest, LargeTuple) {
    std::tuple<int, int, int, int, int> t{1, 2, 3, 4, 5};
    ss << t;
    EXPECT_EQ(ss.str(), "( 1 2 3 4 5 )");
}

//tuple with boolean and char values
TEST_F(TuplePrinterTest, MixedTypesIncludingBoolAndChar) {
    std::tuple<bool, char, int> t{true, 'A', 100};
    ss << t;
    EXPECT_EQ(ss.str(), "( 1 A 100 )");
}