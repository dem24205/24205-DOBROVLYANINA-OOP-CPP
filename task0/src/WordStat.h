#ifndef WORDSTAT_H
#define WORDSTAT_H

#include <string>

class WordStat {
private:
    std::string word;
    std::size_t count;
    double frequency;

public:
    WordStat(const std::string& w, const std::size_t c, const double f = 0.0)
        : word(w), count(c), frequency(f) {}

    const std::string& getWord() const;
    std::size_t getCount() const;
    double getFrequency() const;
};


#endif