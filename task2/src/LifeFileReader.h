#ifndef LIFEFILEREADER_H
#define LIFEFILEREADER_H

#include <string>
#include <fstream>

class LifeFileReader {
private:
    std::string fileName;
    std::ifstream in;
    std::string line;
public:
    explicit LifeFileReader(const std::string& fileName);
    ~LifeFileReader();

    void open();
    void close();

    bool isOpen();
    bool isEOF();
    const std::string& getLine();
};

#endif