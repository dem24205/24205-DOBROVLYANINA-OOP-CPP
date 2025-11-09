#include "SoundProcessorException.h"
#include "WAVHeader.h"
#include <cstring>
#include <ios>
#include <iostream>

int WAVHeader::convertBinStrToInt(const char* string, const int bytes) noexcept {
    int result = 0;
    for (int i = 0; i < bytes; ++i) {
        result |= static_cast<unsigned char>(string[i]) << (i * BYTE_SIZE);  // ← УБРАЛ bytes - i - 1!
    }
    return result;
}

void WAVHeaderParser::parse(const std::string& filename) {
    file.open(filename, std::ios::binary);
    if (!file.is_open()) {
        throw FileException("Cannot open file " + filename);
    }
    char buffer[BUFFER_SIZE + 1];
    buffer[BUFFER_SIZE] = '\0';
    file.read(buffer, 4);
    chunkId = buffer;
    if (strcmp(buffer, "RIFF") != 0) {
        throw FormatException("Incorrect chunk ID");
    }
    file.read(buffer, 4);
    chunkSize = buffer;
    file.read(buffer, 4);
    format = buffer;
    if (strcmp(buffer, "WAVE") != 0) {
        throw FormatException("Incorrect format");
    }
    file.read(buffer, 4);
    subchunk1Id = buffer;
    if (strcmp(buffer, "fmt ") != 0) {
        throw FormatException("Corrupted file");
    }
    file.read(buffer, 4);
    subchunk1Size = convertBinStrToInt(buffer, 4);
    if (subchunk1Size != 16) {
        throw FormatException("Only files in PCM format are supported");
    }
    file.read(buffer, 2);
    audioFormat = convertBinStrToInt(buffer, 2);
    if (audioFormat != 1) {
        throw FormatException("Only files without compression are supported");
    }
    file.read(buffer, 2);
    numChannels = convertBinStrToInt(buffer, 2);
    if (numChannels != 1) {
        throw FormatException("Only files with mono-channel are supported");
    }
    file.read(buffer, 4);
    sampleRate = convertBinStrToInt(buffer, 4);
    //printf("%d\n", sampleRate);
    if (sampleRate != 44100) {
        throw FormatException("Only files with 44100 hz sample rate are supported");
    }
    file.read(buffer, 4);
    byteRate = convertBinStrToInt(buffer, 4);
    file.read(buffer, 2);
    blockAlign = convertBinStrToInt(buffer, 2);
    file.read(buffer, 2);
    bitsPerSample = convertBinStrToInt(buffer, 2);
    if (bitsPerSample != 16) {
        throw FormatException("Only 16 bit per samples are supported");
    }
    file.read(buffer, 4);
    subchunk2Id = buffer;
    if (!strcmp(buffer, "LIST")) {
        file.read(buffer, 4);
        subchunk2Size = convertBinStrToInt(buffer, 4);
        headerSize += subchunk2Size; // attention!
        file.seekg(subchunk2Size, std::ios::cur);
        file.read(buffer, 4);
    }
    if (strcmp(buffer, "data") != 0) {
        throw FormatException("Corrupted file");
    }
    subchunk2Id = buffer;
    file.read(buffer, 4);
    subchunk2Size = convertBinStrToInt(buffer, 4);
    headerSize = static_cast<int>(file.tellg());
    file.close();
}

int WAVHeader::getHeaderSize() const {
    return headerSize;
}

int WAVHeader::getSampleRate() const {
    return sampleRate;
}

int WAVHeader::getSubchunk2Size() const {
    return subchunk2Size;
}

void WAVHeaderWriter::copyFrom(const WAVHeaderParser& parser) {
    chunkId = parser.chunkId;
    chunkSize = parser.chunkSize;
    format = parser.format;
    subchunk1Id = parser.subchunk1Id;
    subchunk1Size = parser.subchunk1Size;
    audioFormat = parser.audioFormat;
    numChannels = parser.numChannels;
    sampleRate = parser.sampleRate;
    byteRate = parser.byteRate;
    blockAlign = parser.blockAlign;
    bitsPerSample = parser.bitsPerSample;
    subchunk2Id = parser.subchunk2Id;
    subchunk2Size = parser.subchunk2Size;
}

void WAVHeaderWriter::writeBinary(std::fstream& out, const int value, const int bytes) {
    out.write(reinterpret_cast<const char*>(&value), bytes);
}

void WAVHeaderWriter::write(std::fstream& out) const {
    //RIFF Chunk Descriptor
    out << chunkId;
    out << chunkSize;
    out << format;
    //"fmt" subchunk
    out << subchunk1Id;
    writeBinary(out, subchunk1Size, 4);
    writeBinary(out, audioFormat, 2);
    writeBinary(out, numChannels, 2);
    writeBinary(out, sampleRate, 4);
    writeBinary(out, byteRate, 4);
    writeBinary(out, blockAlign, 2);
    writeBinary(out, bitsPerSample, 2);
    //"data" subchunk
    out << subchunk2Id;
    writeBinary(out, subchunk2Size, 4);
}
