#ifndef FILEWRITER_H
#define FILEWRITER_H

#include "WordStat.h"
#include <fstream>
#include <string>
#include <vector>

class FileWriter {
private:
    std::string fileName;
    std::ofstream out;
public:
    explicit FileWriter(const std::string& fileName);
    ~FileWriter();
    bool isOpen() const;
    void write(const std::vector<std::string>& wordStat);
};

#endif