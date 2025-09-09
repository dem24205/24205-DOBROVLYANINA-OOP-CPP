#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <fstream>
#include <list>
#include <string>

class FileWriter {
private:
    std::string fileName;
    std::ofstream out;
public:
    FileWriter(const std::string& fileName);
    ~FileWriter();
    bool isOpen() const;
    void writeToFile(const std::list<std::string>& frequencyStat);
};

#endif