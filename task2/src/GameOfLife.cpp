#include "GameOfLife.h"
#include "LifeFileWriter.h"

#include <iostream>
#include <windows.h>
void GameOfLife::start() {
    if (mode == GameMode::Offline) {
        runOffline();
        return;
    }
    runOnline();
}

GameStatus GameOfLife::handleCmd(const Cmd& cmd) {
    const std::string name = cmd.getName();
    const std::string attribute = cmd.getAttribute();
    if (name == "exit") {
        return GameStatus::Exit;
    }
    if (name == "tick" || name == "t") {
        int iterations = 1; // default value
        if (!attribute.empty()) {
            try {
                iterations = std::stoi(attribute);
                if (iterations <= 0) {
                    ConsoleInterface::printError("Iteration count must be positive. Type 'help' for usage information.");
                    return GameStatus::Continue;
                }
            } catch (const std::exception& e) {
                ConsoleInterface::printError("Invalid number format. Type 'help' for usage information.");
                return GameStatus::Continue;
            }
        }
        generalIterations += iterations;
        engine.countGenerations(iterations);
    }
    else if (name == "auto") {
        int iterations = 1;
        if (!attribute.empty()) {
            try {
                iterations = std::stoi(attribute);
                if (iterations <= 0) {
                    ConsoleInterface::printError("Iteration count must be positive. Type 'help' for usage information.");
                    return GameStatus::Continue;
                }
            } catch (const std::exception& e) {
                ConsoleInterface::printError("Invalid number format. Type 'help' for usage information.");
                return GameStatus::Continue;
            }
        }
        for (int i = 0; i < iterations; i++) {
            ++generalIterations;
            engine.countNextGeneration();
            ConsoleInterface::printInterface(engine.getGrid(),
                engine.getBirthRules(), engine.getSurvivalRules(),
                engine.getUniverseName(), generalIterations);
            Sleep(200);
        }
    }
    else if (name == "help") {
        printHelp();
        std::cin.ignore();
    }
    else if (name == "dump") {
        if (attribute.empty()) {
            ConsoleInterface::printError("Missing filename. Type 'help' for usage information.");
            return GameStatus::Continue;
        }
        filename = attribute;
        createLifeFile();
    }
    else {
        ConsoleInterface::printError("Unknown command. Type 'help' for usage information.");
    }
    return GameStatus::Continue;
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