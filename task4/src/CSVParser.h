#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <tuple>
#include <vector>

// tuple getter
template<int Index, typename First, typename... Args>
struct TupleGetter {
    static std::tuple<First, Args...> getTuple(const std::vector<std::string>& line) {
        First temp;
        std::stringstream ss(line[Index]);
        ss >> temp;
        return std::tuple_cat(std::make_tuple(temp), TupleGetter<Index - 1, Args...>::getTuple(line));
    }
};

template<int Index, typename... Args>
struct TupleGetter<Index, std::string, Args...> {
    static std::tuple<std::string, Args...> getTuple(const std::vector<std::string>& line) {
        std::string temp = line[Index];
        return std::tuple_cat(std::make_tuple(temp), TupleGetter<Index - 1, Args...>::getTuple(line));
    }
};

template<typename First, typename... Args>
struct TupleGetter<0, First, Args...>{
    static std::tuple<First, Args...> getTuple(const std::vector<std::string>& line) {
        First temp;
        std::stringstream ss(line[0]);
        ss >> temp;
        return std::make_tuple(temp);
    }
};

template<typename... Args>
struct TupleGetter<0, std::string, Args...> {
    static std::tuple<std::string, Args...> getTuple(const std::vector<std::string>& line) {
        std::string temp = line[0];
        return std::make_tuple(temp);
    }
};

template<typename... Args>
class CSVParser {
    std::istream& in;
    char columnDelim = ',';
    size_t rowCount;
    bool isEof = false;

    std::string getLine() {
        std::string line;
        if (!std::getline(in, line)) {
            isEof = true;
        }
        return line;
    }

    std::vector<std::string> getWords(const std::string& line) {
        std::vector<std::string> result;
        std::stringstream ss(line);
        std::string item;

        while (std::getline(ss, item, columnDelim)) {
            result.push_back(item);
        }
        return result;
    }

    //TODO: написать свои исключения?
    std::tuple<Args...> getTuple() {
        std::string row = getLine();

        if (row.empty() && isEof) {
            throw std::runtime_error("End of file");
        }
        if (row.empty()) {
            throw std::runtime_error("Empty line at row #" + std::to_string(rowCount + 1));
        }

        rowCount++;
        std::vector<std::string> words = getWords(row);

        if (words.size() != sizeof...(Args)) {
            throw std::length_error("Column count mismatch in row #" + std::to_string(rowCount) +
                                  " (expected " + std::to_string(sizeof...(Args)) +
                                  ", got " + std::to_string(words.size()) + ")");
        }

        std::reverse(words.begin(), words.end());
        return TupleGetter<sizeof...(Args) - 1, Args...>::getTuple(words);
    }

public:
    explicit CSVParser(std::istream& in, size_t offset = 0)
        : in(in), rowCount(0) {
        for (size_t i = 0; i < offset; ++i) {
            if (!getLine().empty() && !isEof) {

            }
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

        explicit Iterator(CSVParser* parser, bool isEnd = false)
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