#ifndef FILEWRITER_H
#define FILEWRITER_H

#include "WordStat.h"
#include <fstream>
#include <string>
#include <vector>

class CSVFileWriter {
private:
    std::string fileName;
    std::ofstream out;
public:
    explicit CSVFileWriter(const std::string& fileName);
    ~CSVFileWriter();
    bool isOpen() const;
    void write(const std::vector<std::string>& wordStat);
};

#endif