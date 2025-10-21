#ifndef COMMAND_H
#define COMMAND_H

#include <memory>
#include "ConsoleInterface.h"

class GameOfLife;

enum class GameStatus {Continue, Exit};

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual GameStatus execute(GameOfLife& game) = 0;
};

class CommandFactory {
public:
    static std::unique_ptr<ICommand> createCommand(const Cmd& cmd);
};

#endif