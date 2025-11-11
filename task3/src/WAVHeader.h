#ifndef WAVHEADER_H
#define WAVHEADER_H

#include <fstream>
#include <string>

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
    std::string subchunk1Id = "fmt ";
    int subchunk1Size = 16;
    int audioFormat = 1;
    int numChannels = 1;
    int sampleRate = 44100;
    int byteRate{};
    int blockAlign{};
    int bitsPerSample = 16;
    //data subchunk
    std::string subchunk2Id = "data";
    int subchunk2Size = 0;
    int headerSize = 0;
    static int convertBinStrToInt(const char* string, int bytes) noexcept;
public:
    WAVHeader() = default;
    int getHeaderSize() const;
    int getSampleRate() const;
    int getSubchunk2Size() const;
};

class WAVHeaderParser : public WAVHeader {
public:
    friend class WAVHeaderWriter;
    void parse(const std::string& filename);
};

class WAVHeaderWriter : public WAVHeader {
private:
    static void writeBinary(std::fstream& out, int value, int bytes);
public:
    void copyFrom(const WAVHeaderParser& Parser);
    void write(std::fstream &out) const;
};

#endif