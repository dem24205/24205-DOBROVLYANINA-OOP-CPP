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

class TickCommand : public ICommand {
private:
    int iteration;
public:
    explicit TickCommand(const std::string& attr);
    GameStatus execute(GameOfLife& game) override;
};

class AutoCommand : public ICommand {
private:
    int iteration;
public:
    explicit AutoCommand(const std::string& attr);
    GameStatus execute(GameOfLife& game) override;
};

class ExitCommand : public ICommand {
public :
    GameStatus execute(GameOfLife& game) override;
};

class HelpCommand : public ICommand {
public:
    GameStatus execute(GameOfLife& game) override;
};

class DumpCommand : public ICommand {
private:
    std::string filename;
public:
    explicit DumpCommand(const std::string& filename) : filename(filename) {};
    GameStatus execute(GameOfLife& game) override;
};

class CommandFactory {
public:
    static std::unique_ptr<ICommand> createCommand(const Cmd &cmd);
};

#endif