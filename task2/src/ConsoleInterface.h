#ifndef CONSOLEINTERFACE_H
#define CONSOLEINTERFACE_H

#include <string>
#include <vector>

class Cmd {
private:
    std::string name;
    std::string attribute;
public:
    explicit Cmd(const std::string& cmd);
    std::string getName() const;
    std::string getAttribute() const;
};

class ConsoleInterface {
private:
    static inline std::vector<std::string> warnings;
public:
    static Cmd getCommand();
    static void printInterface(const std::vector<std::vector<bool>>& grid,
        const std::vector<int>& birthRules,
        const std::vector<int>& survivalRules,
        const std::string& nameUniverse,
        int iterations);
    static void printWarning(const std::string& msg);
    static void printError(const std::string& msg);
    static void flushWarnings();
};

void printHelp();
#endif