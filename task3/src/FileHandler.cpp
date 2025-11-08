#include "FileHandler.h"
#include "SoundProcessorException.h"
#include "WAVHeader.h"
#include <iostream>

FileHandler::FileHandler(const std::string &inputFilename, const std::string &outputFilename) {
    WAVHeaderParser inputParser;
    inputParser.parse(inputFilename);
    inputHeaderEnd = inputParser.getHeaderSize();
    isSameFile = inputFilename == outputFilename;
    if (!isSameFile) {
        input.open(inputFilename, std::ios::binary);
        if (!input.is_open()) {
            throw FileException("Cannot open input file");
        }
    }
    output.open(outputFilename, std::ios::binary | std::ios::in | std::ios::out);
    //move in helping func
    const bool isOutputOpen = output.is_open() && output.peek() != -1;
    WAVHeaderParser outputParser;
    if (isOutputOpen) {
        outputParser.parse(outputFilename);
        outputHeaderEnd = outputParser.getHeaderSize();
    }
    else {
        output.open(outputFilename, std::ios::binary | std::ios::out);
    }
    if (!isOutputOpen) {
        WAVHeaderWriter headerWriter;
        headerWriter.copyFrom(inputParser);
        headerWriter.write(output);
    }
    output.seekg(inputHeaderEnd, std::ios::beg);
    output.seekp(inputHeaderEnd, std::ios::beg);
    inputSubchunk2Size = inputParser.getSubchunk2Size();
    if (isOutputOpen) {
        outputSubchunk2Size = outputParser.getSubchunk2Size();
    }
    sampleRate = inputParser.getSampleRate();
}

FileHandler::~FileHandler() {
    if (isSameFile) {
        input.close();
    }
    output.close();
}

bool FileHandler::isInPlace() const {
    return isSameFile;
}

void FileHandler::seekToDataStart() {
    input.seekg(inputHeaderEnd, std::ios::beg);
    if (outputHeaderEnd > 0) {
        output.seekg(outputHeaderEnd, std::ios::beg);
        output.seekp(outputHeaderEnd, std::ios::beg);
    }
}

int FileHandler::getOutputDataSize() const {
    return outputSubchunk2Size;
}

int FileHandler::getInputDataSize() const {
    return inputSubchunk2Size;
}

int FileHandler::getOutputDataOffset() const {
    return outputHeaderEnd;
}

int FileHandler::getInputDataOffset() const {
    return inputHeaderEnd;
}

void FileHandler::moveWriterPointer(const int offset) {
    output.seekp(offset, std::ios::cur);
}

void FileHandler::moveReaderPointer(const int offset) {
   input.seekg(offset, std::ios::cur);
}

int FileHandler::getSampleRate() const {
    return sampleRate;
}

void FileWriter::write(std::ostream &output, const char *buffer, const int size) {
    output.write(buffer, size);
}

void FileReader::read(std::istream& input, char* buffer, const int size) {
    input.read(buffer, size);
}

char* FileHandler::getStreamFromIn() {
    char* buffer = new char[SECOND];
    FileReader::read(input, buffer, SECOND);
    return buffer;
}

char* FileHandler::getStreamFromOut() {
    char* buffer = new char[SECOND];
    FileReader::read(output, buffer, SECOND);
    return buffer;
}

void FileHandler::writeStream(const char* buffer, const int offset) {
    moveWriterPointer(offset);
    FileWriter::write(output, buffer, SECOND);
}