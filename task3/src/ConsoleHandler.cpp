#include "ConsoleHandler.h"
#include "SoundProcessorException.h"
#include <cstring>

RunMode ConsoleHandler::parseConsoleCommand(const int argc, char* argv[]) {
    if (argc < 2) {
        throw ConfigException("No console command.");
    }
    if (!strcmp(argv[1], "-h")) {
        if (argc != 2) {
            throw ConfigException("Invalid console command.");
        }
        return RunMode::Help;
    }
    if (argc <= 4) {
        throw ConfigException("Not enough arguments to run.");
    }
    if (strcmp(argv[1], "-c") != 0) {
        throw ConfigException("Invalid console command.");
    }
    configFilename = argv[2];
    for (int i = 3; i < argc; ++i) {
        if (!isWavFile(argv[i])) {
            throw ConfigException("All audio files should be .wav format");
        }
        inputFilenames.emplace_back(argv[i]);
    }
    return RunMode::Run;
}

std::string ConsoleHandler::getConfigFilename() const {
    return configFilename;
}

std::vector<std::string> ConsoleHandler::getFilenames() const {
    return inputFilenames;
}

bool ConsoleHandler::isWavFile(const char* str) {
    if (!str) return false;
    const size_t len = strlen(str);
    if (len < 5) return false;
    return (strcmp(str + len - 4, ".wav") == 0);
}