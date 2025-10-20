#ifndef GAMEOFLIFE_H
#define GAMEOFLIFE_H

#include "ConsoleInterface.h"
#include "GameConfig.h"
#include "GameEngine.h"

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