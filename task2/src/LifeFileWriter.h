#ifndef LIFEFILEWRITER_H
#define LIFEFILEWRITER_H

#include <string>
#include <fstream>

class LifeFileWriter {
private:
    std::string fileName;
    std::ofstream file;
public:
    explicit LifeFileWriter(const std::string& userFileName);
    ~LifeFileWriter();
    void printString(const std:: string& formatLine);
    void printValue(int value);
};

#endif