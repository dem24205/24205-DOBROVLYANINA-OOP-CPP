#ifndef CONSOLEHANDLER_H
#define CONSOLEHANDLER_H

#include <string>
#include <vector>

enum class RunMode {Help, Run};

class ConsoleHandler {
private:
    std::vector<std::string> filenames;
    std::string configFilename;
    static bool isWavFile(const char* str);
public:
    RunMode parseConsoleCommand(int argc, char* argv[]);
    const std::vector<std::string>& getFilenames() const;
    const std::string& getConfigFilename() const;
};

#endif