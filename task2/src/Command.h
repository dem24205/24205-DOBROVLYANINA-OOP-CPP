#ifndef COMMAND_H
#define COMMAND_H

#include "ConsoleInterface.h"
#include <memory>

class LifeGame;

enum class GameStatus {Continue, Exit};

class Command {
public:
    virtual ~Command() = default;
    virtual GameStatus execute(LifeGame& game) = 0;
};

class TickCommand : public Command {
private:
    int isCorrectAttr = false;
    int iteration;
public:
    explicit TickCommand(const std::string& attr);
    GameStatus execute(LifeGame& game) override;
};

class AutoCommand : public Command {
private:
    int isCorrectAttr = false;
    int iteration;
public:
    explicit AutoCommand(const std::string& attr);
    GameStatus execute(LifeGame& game) override;
};

class ExitCommand : public Command {
public :
    GameStatus execute(LifeGame& game) override;
};

class HelpCommand : public Command {
public:
    GameStatus execute(LifeGame& game) override;
};

class DumpCommand : public Command {
private:
    std::string filename;
public:
    explicit DumpCommand(const std::string& filename) : filename(filename) {};
    GameStatus execute(LifeGame& game) override;
};

class CommandFactory {
public:
    static std::unique_ptr<Command> createCommand(const Cmd &cmd);
};

#endif