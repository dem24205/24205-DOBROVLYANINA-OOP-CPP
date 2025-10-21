#include "Command.h"
#include "GameOfLife.h"
#include "ConsoleInterface.h"
#include <iostream>
#include <windows.h>

class TickCommand : public ICommand {
private:
    int iterations = 1;
public:
    explicit TickCommand(const std::string& attr) {
        try {
            iterations = std::stoi(attr);
        } catch (...) {
            iterations = 1;
        }
        if (iterations <= 0) {
            iterations = 1;
        }
    };

    GameStatus execute(GameOfLife& game) override {
        game.engine.countGenerations(iterations);
        game.generalIterations += iterations;  // ← ДОБАВЬ ЭТУ СТРОЧКУ!
        return GameStatus::Continue;
    }
};

class AutoCommand : public ICommand {
private:
    int iterations = 1;
public:
    explicit AutoCommand(const std::string& attr) {
        try {
            iterations = std::stoi(attr);
        } catch (...) {
            iterations = 1;
        }
        if (iterations <= 0) {
            iterations = 1;
        }
    };

    GameStatus execute(GameOfLife& game) override {
        for (int i = 0; i < iterations; i++) {
            ++game.generalIterations;
            game.engine.countNextGeneration();
            ConsoleInterface::printInterface(game.engine.getGrid(),
                game.engine.getBirthRules(), game.engine.getSurvivalRules(),
                game.engine.getUniverseName(), game.generalIterations);
            Sleep(200);
        }
        return GameStatus::Continue;
    }
};


class DumpCommand : public ICommand {
private:
    std::string filename;
public:
    explicit DumpCommand(const std::string& userFilename) : filename(userFilename) {};
    GameStatus execute(GameOfLife& game) override {
        game.filename = filename;
        game.createLifeFile();
        return GameStatus::Continue;
    }
};

class HelpCommand : public ICommand {
private:
public:
    GameStatus execute(GameOfLife& game) override {
        printHelp();
        std::cin.ignore();
        return GameStatus::Continue;
    }
};

class ExitCommand : public ICommand {
public:
    GameStatus execute(GameOfLife& game) override {
        return GameStatus::Exit;
    }
};

std::unique_ptr<ICommand> CommandFactory::createCommand(const Cmd& cmd) {
    std::string name = cmd.getName();
    std::string attr = cmd.getAttribute();

    if (name == "tick" || name == "t") {
        return std::make_unique<TickCommand>(attr);
    }
    if (name == "auto") {
        return std::make_unique<AutoCommand>(attr);
    }
    if (name == "dump") {
        return std::make_unique<DumpCommand>(attr);
    }
    if (name == "help") {
        return std::make_unique<HelpCommand>();
    }
    if (name == "exit") {
        return std::make_unique<ExitCommand>();
    }
    // TODO: разобрать наследование, override, config = handleParsing();
    //TODO: grid:: static return snomFlakeGrid()?
    return nullptr;
}