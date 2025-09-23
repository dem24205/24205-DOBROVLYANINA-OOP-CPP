#include "CSVFileWriter.h"
#include <string>

CSVFileWriter::CSVFileWriter(const std::string &fileName) {
    this->fileName = fileName;
    out.open(fileName);
}

CSVFileWriter::~CSVFileWriter() {
    out.close();
}

bool CSVFileWriter::isOpen() const{
    return out.is_open();
}

void CSVFileWriter::write(const std::vector<std::string>& values) {
    for (size_t i = 0; i < values.size(); ++i) {
        out << values[i];
        if (i != values.size() - 1) {
            out << ",";
        }
    }
    out << "\n";
    out.flush();
}