#include "FileWriter.h"
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


void FileWriter::write(const std::vector<std::string>& values) {
    for (size_t i = 0; i < values.size(); ++i) {
        out << values[i];
        if (i != values.size() - 1) {
            out << ",";  // Разделитель между значениями
        }
    }
    out << "\n";
    out.flush();
}