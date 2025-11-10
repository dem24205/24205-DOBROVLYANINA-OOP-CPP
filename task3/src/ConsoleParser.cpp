#include "ConsoleParser.h"
#include "SoundProcessorException.h"
#include <cstring>

RunMode ConsoleParser::parse(const int argc, char* argv[]) {
    if (argc < 2) {
        throw ConsoleException("No console command");
    }
    const std::string mode = argv[1];

    if (mode == "-h") {
        if (argc != 2) {
            throw ConsoleException("Invalid console command");
        }
        return RunMode::Help;
    }

    if (mode != "-c") {
        throw ConsoleException("Invalid console command");
    }

    if (argc <= 4) {
        throw ConsoleException("Not enough arguments to run");
    }

    configFilename = argv[2];
    for (int i = 3; i < argc; ++i) {
        if (!isWavFile(argv[i])) {
            throw ConsoleException("Only wav files are supported");
        }
        filenames.emplace_back(argv[i]);
    }
    return RunMode::Run;
}

const std::string& ConsoleParser::getConfigFilename() const {
    return configFilename;
}

const std::vector<std::string>& ConsoleParser::getFilenames() const {
    return filenames;
}

bool ConsoleParser::isWavFile(const char* str) {
    if (!str) return false;
    const size_t len = strlen(str);
    if (len < 5) return false;
    return (strcmp(str + len - 4, ".wav") == 0);
}