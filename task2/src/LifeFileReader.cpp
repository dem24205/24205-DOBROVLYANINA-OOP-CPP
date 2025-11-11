#include "LifeFileReader.h"

LifeFileReader::LifeFileReader(const std::string &fileName) {
    this->fileName = fileName;
}

LifeFileReader::~LifeFileReader() {
    close();
}

void LifeFileReader::open() {
    in.open(fileName);
}

void LifeFileReader::close() {
    in.close();
}

bool LifeFileReader::isOpen() {
    return in.is_open();
}

const std::string &LifeFileReader::getLine() {
    std::getline(in, line);
    return line;
}

bool LifeFileReader::isEOF() {
    return in.peek() == std::char_traits<char>::eof();
}