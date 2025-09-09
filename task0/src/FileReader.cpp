#include "FileReader.h"

FileReader::FileReader(const std::string &fileName) {
    this->fileName = fileName;
    in.open(fileName);
}

FileReader::~FileReader() {
    in.close();
}

bool FileReader::isOpen() {
    return in.is_open();
}

const std::string &FileReader::getLine() {
    std::getline(in, line);
    return line;
}

bool FileReader::isEOF() {
    return in.peek() == std::char_traits<char>::eof();
}