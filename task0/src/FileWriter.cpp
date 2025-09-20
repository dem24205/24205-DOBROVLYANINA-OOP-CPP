#include "FileWriter.h"
#include <iomanip>
#include <sstream>
#include <string>

FileWriter::FileWriter(const std::string &fileName) {
    this->fileName = fileName;
    out.open(fileName);
}

FileWriter::~FileWriter() {
    out.close();
}

bool FileWriter::isOpen() const{
    return out.is_open();
}

static std::string convertToString(const WordStat& item) {
    std::ostringstream os;
    os << item.getWord() << ';' << item.getCount() << ';' << std::fixed << std::setprecision(3) << item.getFrequency();
    return os.str();
}

void FileWriter::writeToFile(const std::vector<WordStat> &wordStats) {
    out << "word;count;percent\n";
    for (const auto& word : wordStats) {
        out << convertToString(word) << "\n";
    }
    out.flush();
}