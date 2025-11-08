#ifndef SOUNDPROCESSOR_H
#define SOUNDPROCESSOR_H

#include <fstream>
#include <string>
#include <vector>

class SoundProcessor {
private:
    std::vector<std::string> filenames;
    std::ifstream config;
    std::string configFilename;
    static void initializeOutput(const std::string& outputName, const std::string& firstInput);
public:
    SoundProcessor() = default;
    SoundProcessor(const std::string& configFilename, const std::vector<std::string>& filenames)
    : filenames(filenames), configFilename(configFilename) {};
    void run();
    static void printHelp();
};

#endif