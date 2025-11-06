#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <fstream>

class FileHandler {
private:
    const int SECOND = 44100 * 2;
    std::string inputFilename;
    std::string outputFilename;
    std::ifstream input;
    std::fstream output;
    int inputHeaderEnd;
    int outputHeaderEnd{};
    int inputSubchunk2Size;
    int outputSubchunk2Size{};
    int sampleRate;
    bool isSameFile;
public:
    FileHandler(const std::string& inputFilename, const std::string& outputFilename);
    ~FileHandler();
    bool isInPlace() const;
    void seekToDataStart();
    int getOutputDataSize() const;
    int getInputDataSize() const;
    int getOutputDataOffset() const;
    int getInputDataOffset() const;
    void moveWriterPointer(int offset);
    void moveReaderPointer(int offset);
    char* getStreamFromIn();
    char* getStreamFromOut();
    int getSampleRate() const;
    void writeStream(const char* buffer, int offset);
};

class FileReader {
public:
    static void read(std::istream& input, char* buffer, int size);
};

class FileWriter {
public:
    static void write(std::ostream& output, const char* buffer, int size);
};

#endif