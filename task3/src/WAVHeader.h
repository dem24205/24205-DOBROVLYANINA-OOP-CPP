#ifndef WAVHEADER_H
#define WAVHEADER_H

#include <string>
#include <fstream>

class WAVHeader {
protected:
    static constexpr int BYTE_SIZE = 8;
    const int BUFFER_SIZE = 4;
    std::ifstream file;
    //RIFF Chunk Descriptor
    std::string chunkId = "RIFF";
    std::string chunkSize = "0000";
    std::string format = "WAVE";
    //fmt subchunk
    std::string subChunk1Id = "fmt ";
    int subChunk1Size = 16;
    int audioFormat = 1;
    int numChannels = 1;
    int sampleRate = 44100;
    int byteRate{};
    int blockAlign{};
    int bitsPerSample = 16;
    //data subchunk
    std::string subChunk2Id = "data";
    int subChunk2Size = 0;
    int dataStartPos = 0;
    static int convertBinStrToInt(const char* string, int bytes) noexcept;
public:
    WAVHeader() = default;
};

class WAVHeaderParser : public WAVHeader {
public:
    void parse(const std::string& filename);
};


class WAVHeaderWriter : public WAVHeader {
private:
    static void writeBinary(std::ofstream& out, int value, int bytes);
public:
    void write(std::ofstream &out) const;
};

#endif