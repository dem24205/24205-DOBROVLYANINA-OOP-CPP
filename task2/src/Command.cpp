#include "Command.h"
#include "GameOfLife.h"
#include "ConsoleInterface.h"
#include <iostream>
#include <windows.h>

::TickCommand::TickCommand(const std::string &attr) {
    try {
        iteration = std::stoi(attr);
    }
    catch (...) {
        iteration = 1;
    }
    if (iteration <= 0) {
        iteration = 1;
    }
}

GameStatus TickCommand::execute(GameOfLife &game) {
    game.engine.countGenerations(iteration);
    game.generalIterations += iteration;
    return GameStatus::Continue;
}

::AutoCommand::AutoCommand(const std::string &attr) {
    try {
        iteration = std::stoi(attr);
    }
    catch (...) {
        iteration = 1;
    }
    if (iteration <= 0) {
        iteration = 1;
    }
}

GameStatus AutoCommand::execute(GameOfLife &game) {
    for (int i = 0; i < iteration; i++) {
        ++game.generalIterations;
        game.engine.countNextGeneration();
        ConsoleInterface::printInterface(game.engine.getGrid(),
            game.engine.getBirthRules(), game.engine.getSurvivalRules(),
            game.engine.getUniverseName(), game.generalIterations);
        Sleep(200);
    }
    return GameStatus::Continue;
}

GameStatus DumpCommand::execute(GameOfLife &game) {
    game.filename = filename;
    game.createLifeFile();
    return GameStatus::Continue;
}


GameStatus HelpCommand::execute(GameOfLife &game) {
    printHelp();
    std::cin.ignore();
    return GameStatus::Continue;
}

GameStatus ExitCommand::execute(GameOfLife &game) {
    return GameStatus::Exit;
}

std::unique_ptr<ICommand> CommandFactory::createCommand(const Cmd &cmd) {
    const std::string name = cmd.getName();
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