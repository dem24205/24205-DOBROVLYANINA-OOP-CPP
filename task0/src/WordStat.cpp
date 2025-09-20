#include "WordStat.h"

std::size_t WordStat::getCount() const {
    return count;
}
double WordStat::getFrequency() const {
    return frequency;
}
const std::string &WordStat::getWord() const {
    return word;
}

