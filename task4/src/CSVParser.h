#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

//CSV parser exception
class CSVParserException : public std::runtime_error {
public:
    explicit CSVParserException(const std::string& message)
        : std::runtime_error(message) {}
};

static void throwCSVParseError(size_t row, size_t column, const std::string& message) {
    std::ostringstream oss;
    oss << "Parse error at row " << row << ", column " << column << ": " << message;
    throw CSVParserException(oss.str());
}

//tuple getter
template<int Index, typename First, typename... Args>
struct TupleGetter {
    static std::tuple<First, Args...> getTuple(const std::vector<std::string>& line, size_t rowCount) {
        constexpr size_t totalColumns = 1 + sizeof...(Args);
        First temp;
        std::stringstream ss(line[Index]);
        ss >> temp;
        if (ss.fail() || !ss.eof()) {
            size_t realColumn = totalColumns - Index;
            std::ostringstream oss;
            oss << "Failed to convert \"" << line[Index] << "\" to required type";
            throwCSVParseError(rowCount, realColumn, oss.str());
        }
        return std::tuple_cat(std::make_tuple(temp),
            TupleGetter<Index - 1, Args...>::getTuple(line, rowCount));
    }
};

template<int Index, typename... Args>
struct TupleGetter<Index, std::string, Args...> {
    static std::tuple<std::string, Args...> getTuple(const std::vector<std::string>& line, size_t rowCount) {
        std::string temp = line[Index];
        return std::tuple_cat(std::make_tuple(temp), TupleGetter<Index - 1, Args...>::getTuple(line, rowCount));
    }
};

template<typename First, typename... Args>
struct TupleGetter<0, First, Args...>{
    static std::tuple<First, Args...> getTuple(const std::vector<std::string>& line, size_t rowCount) {
        constexpr size_t totalColumns = 1 + sizeof...(Args);
        First temp;
        std::stringstream ss(line[0]);
        ss >> temp;
        if (ss.fail() || !ss.eof()) {
            std::ostringstream oss;
            oss << "Failed to convert \"" << line[0] << "\" to required type";
            throwCSVParseError(rowCount, totalColumns, oss.str());
        }
        return std::make_tuple(temp);
    }
};

template<typename... Args>
struct TupleGetter<0, std::string, Args...> {
    static std::tuple<std::string, Args...> getTuple(const std::vector<std::string>& line, size_t rowCount) {
        std::string temp = line[0];
        return std::make_tuple(temp);
    }
};

//CSV parser config
struct CSVParserConfig {
    char columnDelim = ',';
    char rowDelim = '\n';
    char escapeChar = '"';
    CSVParserConfig() = default;
    explicit CSVParserConfig(const char cDelim, const char rDelim, const char escape = '"') :
    columnDelim(cDelim), rowDelim(rDelim), escapeChar(escape) {}
};

//CSV parser
template<typename... Args>
class CSVParser {
    std::istream& in;
    CSVParserConfig config;
    size_t rowCount;
    bool isEof = false;

    std::string getLine() {
        std::string line;
        if (config.rowDelim == '\n') { //default
            if (!std::getline(in, line)) {
                isEof = true;
            }
        }
        else {
            char ch;
            while (in.get(ch) && ch != config.rowDelim) {
                line += ch;
            }
            if (!in && line.empty()) {
                isEof = true;
            }
        }
        return line;
    }

    std::vector<std::string> getWords(const std::string& line) {
        std::vector<std::string> result;
        std::string current;
        bool inQuotes = false;
        bool justClosedQuote = false;
        size_t column = 1;

        for (size_t i = 0; i < line.size(); ++i) {
            char ch = line[i];
            if (ch == config.escapeChar) {
                if (inQuotes) {
                    if (i + 1 < line.size() && line[i + 1] == config.escapeChar) {
                        current += config.escapeChar;
                        ++i;
                    } else {
                        inQuotes = false;
                        justClosedQuote = true;
                    }
                } else {
                    if (!current.empty()) {
                        throwCSVParseError(rowCount, column, "Unexpected quote inside unquoted field");
                    }
                    inQuotes = true;
                }
            } else if (ch == config.columnDelim && !inQuotes) {
                result.push_back(current);
                current.clear();
                justClosedQuote = false;
                ++column;
            } else {
                if (justClosedQuote) {
                    throwCSVParseError(rowCount, column, "Unexpected characters after closing quote");
                }
                current += ch;
            }
        }

        if (inQuotes) {
            throwCSVParseError(rowCount, column, "Unterminated quoted field");
        }

        result.push_back(current);
        return result;
    }

    std::tuple<Args...> getTuple() {
        std::string row = getLine();
        if (row.empty() && isEof) {
            throw CSVParserException("End of file");
        }
        if (row.empty()) {
            throw CSVParserException("Empty line at row #" + std::to_string(rowCount + 1));
        }
        rowCount++;
        std::vector<std::string> words = getWords(row);
        if (words.size() != sizeof...(Args)) {
            throw CSVParserException("Column count mismatch in row #" + std::to_string(rowCount) +
                                  " (expected " + std::to_string(sizeof...(Args)) +
                                  ", got " + std::to_string(words.size()) + ")");
        }
        std::reverse(words.begin(), words.end());
        return TupleGetter<sizeof...(Args) - 1, Args...>::getTuple(words, rowCount);
    }

public:
    explicit CSVParser(std::istream& in, size_t offset = 0)
        : in(in), rowCount(0) {
        for (size_t i = 0; i < offset; ++i) {
            if (!getLine().empty() && !isEof) {

            }
        }
    }

    explicit CSVParser(std::istream& in, const CSVParserConfig& config, size_t offset = 0)
        : in(in), config(config), rowCount(0) {
        for (size_t i = 0; i < offset; ++i) {
            if (!getLine().empty() && !isEof) {}
        }
    }

    // Input iterator
    class Iterator {
        CSVParser* parser;
        std::tuple<Args...> current;
        bool isEnd;

    public:
        using iterator_category = std::input_iterator_tag;
        using value_type = std::tuple<Args...>;
        using difference_type = std::ptrdiff_t;
        using pointer = std::tuple<Args...>*;
        using reference = std::tuple<Args...>&;

        explicit Iterator(CSVParser* parser, const bool isEnd = false)
            : parser(parser), isEnd(isEnd) {
            if (!isEnd) {
                ++(*this);
            }
        }

        Iterator& operator++() {
            if (isEnd) return *this;
            try {
                current = parser->getTuple();
            } catch (const std::runtime_error& e) {
                if (std::string(e.what()).find("End of file") != std::string::npos) {
                    isEnd = true;
                } else {
                    throw;
                }
            }
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return isEnd != other.isEnd;
        }

        bool operator==(const Iterator& other) const {
            return isEnd == other.isEnd;
        }

        std::tuple<Args...>& operator*() {
            return current;
        }

        std::tuple<Args...>* operator->() {
            return &current;
        }
    };

    Iterator begin() {
        return Iterator(this);
    }

    Iterator end() {
        return Iterator(this, true);
    }
};

#endif