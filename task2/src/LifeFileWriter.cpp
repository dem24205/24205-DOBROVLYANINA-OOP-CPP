#include "LifeFileWriter.h"

//вынести open в отдельную функцию может быть?
LifeFileWriter::LifeFileWriter(const std::string& userFileName) :fileName(userFileName) {
    file.open(fileName);
}

LifeFileWriter::~LifeFileWriter() {
    file.close();
}

void LifeFileWriter::printString(const std::string& formatLine) {
    file << formatLine;
}

void LifeFileWriter::printValue(const int value) {
    file << value;
}