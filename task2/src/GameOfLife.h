#ifndef GAMEOFLIFE_H
#define GAMEOFLIFE_H

#include "GameConfig.h"
#include "GameEngine.h"
#include "ConsoleInterface.h"
#include "Command.h"  // теперь здесь GameStatus и ICommand

class GameOfLife {
private:
    GameEngine engine;
    GameMode mode;
    GameVersion version;
    int generalIterations = 0;
    int offlineIterations = 0;
    std::string filename{};
    void runOnline();
    void runOffline();
    void createLifeFile();
    friend class TickCommand;  // нужно будет реализовать команды в .cpp
    friend class AutoCommand;
    friend class DumpCommand;
    friend class ExitCommand;
public:
    explicit GameOfLife(const GameConfig& config)
        : engine(config),
        mode(config.getMode()),
        version(config.getVersion()),
        offlineIterations(config.getIterations()),
        filename(config.getOutputFile()) {};
    GameStatus handleCmd(const Cmd& cmd);
    void start();
};

#endif