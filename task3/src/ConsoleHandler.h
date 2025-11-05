#ifndef CONSOLEHANDLER_H
#define CONSOLEHANDLER_H
#include <string>
#include <vector>

enum class RunMode {Help, Run};

class ConsoleHandler {
private:
    std::string configFilename;
    std::vector<std::string> inputFilenames;

    static bool isWavFile(const char* str);
public:
    RunMode parseConsoleCommand(int argc, char* argv[]);
    std::vector<std::string> getFilenames() const;
    std::string getConfigFilename() const;
};

#endif