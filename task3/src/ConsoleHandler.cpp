#include "ConsoleHandler.h"
#include "SoundProcessorException.h"
#include <cstring>

RunMode ConsoleHandler::parseConsoleCommand(const int argc, char* argv[]) {
    if (argc < 2) {
        throw ConfigException("No console command");
    }
    const std::string mode = argv[1];

    if (mode == "-h") {
        if (argc != 2) {
            throw ConfigException("Invalid console command");
        }
        return RunMode::Help;
    }

    if (mode != "-c") {
        throw ConfigException("Invalid console command");
    }

    if (argc <= 4) {
        throw ConfigException("Not enough arguments to run");
    }

    configFilename = argv[2];
    for (int i = 3; i < argc; ++i) {
        if (!isWavFile(argv[i])) {
            throw ConfigException("Only wav files are supported");
        }
        filenames.emplace_back(argv[i]);
    }
    return RunMode::Run;
}

const std::string& ConsoleHandler::getConfigFilename() const {
    return configFilename;
}

const std::vector<std::string>& ConsoleHandler::getFilenames() const {
    return filenames;
}

bool ConsoleHandler::isWavFile(const char* str) {
    if (!str) return false;
    const size_t len = strlen(str);
    if (len < 5) return false;
    return (strcmp(str + len - 4, ".wav") == 0);
}