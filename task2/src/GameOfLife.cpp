#include "GameOfLife.h"
#include "LifeFileWriter.h"
#include <iostream>

void GameOfLife::start() {
    if (mode == GameMode::Offline) {
        runOffline();
        return;
    }
    runOnline();
}

GameStatus GameOfLife::handleCmd(const Cmd& cmd) {
    auto command = CommandFactory::createCommand(cmd);

    if (!command) {
        ConsoleInterface::printError("Unknown command. Type 'help' for usage.");
        return GameStatus::Continue;
    }

    return command->execute(*this);
}

void GameOfLife::runOnline() {
    auto status = GameStatus::Continue;
    while (status == GameStatus::Continue) {
        ConsoleInterface::flushWarnings();
        ConsoleInterface::printInterface(engine.getGrid(),
        engine.getBirthRules(), engine.getSurvivalRules(),
        engine.getUniverseName(), generalIterations);
        Cmd cmd = ConsoleInterface::getCommand();
        status = handleCmd(cmd);
    }
}

void GameOfLife::createLifeFile() {
    filename += ".life";
    LifeFileWriter writer(filename);
    writer.printString("#Life 1.06\n");
    writer.printString("#N " + engine.getUniverseName() + '\n');
    writer.printString("#R ");
    std::string string;
    writer.printString("B");
    for (const auto i : engine.getBirthRules()) {
        string += std::to_string(i);
    }
    string += "/S";
    for (const auto i : engine.getSurvivalRules()) {
        string += std::to_string(i);
    }
    writer.printString(string + '\n');
    writer.printString( "#S C");
    writer.printValue(engine.getHeight());
    writer.printString( "/R");
    writer.printValue(engine.getWidth());
    writer.printString( "\n");
    const std::vector<std::vector<bool>> aliveCell = engine.getGrid();
    for (int i = 0; i < aliveCell.size(); ++i) {
        for (int j = 0; j < aliveCell[0].size(); ++j) {
            if (aliveCell[i][j]) {
                writer.printValue(j);
                writer.printString(" ");
                writer.printValue(i);
                writer.printString("\n");
            }
        }
    }
}

void GameOfLife::runOffline() {
    ConsoleInterface::flushWarnings();
    engine.countGenerations(offlineIterations);
    createLifeFile();
}