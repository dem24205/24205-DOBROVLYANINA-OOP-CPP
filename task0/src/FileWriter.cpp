#include "FileWriter.h"
#include <filesystem>

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

void FileWriter::writeToFile(const std::list<std::string>& frequencyStat) {
    out << "word;count;percent\n";
    for (const std::string& row : frequencyStat) {
        out << row << '\n';
    }
    out.flush();
}