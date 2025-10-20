#include "ConsoleInterface.h"
#include <iostream>
#include <vector>

Cmd::Cmd(const std::string &cmd) {
    const size_t firstSpace = cmd.find(' ');
    if (firstSpace == std::string::npos) {
        name = cmd;
        attribute = "";
        return;
    }
    name = cmd.substr(0, firstSpace);
    attribute = cmd.substr(firstSpace + 1);
    size_t start = attribute.find_first_not_of(" ");
    size_t end = attribute.find_last_not_of(" ");
    if (start != std::string::npos && end != std::string::npos) {
        attribute = attribute.substr(start, end - start + 1);
    } else {
        attribute = "";
    }
}

std::string Cmd::getName() const {
    return name;
}

std::string Cmd::getAttribute() const {
    return attribute;
}

void renderGrid(const std::vector<std::vector<bool>>& grid) {
    std::cout << " ";
    const size_t width = grid[0].size();
    const size_t height = grid.size();
    for (size_t j = 0; j < width; ++j) {
        std::cout << "__";
    }
    std::cout << "\n";
    for (int i = 0; i < height; ++i) {
        std::cout << '|';
        for (int j = 0; j < grid[i].size(); ++j) {
            if (grid[i][j])
                std::cout << "#" << ' ';
            else {
                std::cout << " " << ' ';
            }
        }
        std::cout << "|\n";
    }
    std::cout << " ";
    for (int j = 0; j < width; ++j) {
        std::cout << "‾‾";
    }
    std::cout << "\n> ";
}

void ConsoleInterface::printInterface(const std::vector<std::vector<bool>> &grid,
    const std::vector<int> &birthRules,
    const std::vector<int> &survivalRules,
    const std::string &nameUniverse,
    const int iterations) {
    system("cls");
    std::cout << "Universe name: " << nameUniverse << std::endl;
    std::cout << "B";
    for (const auto rule : birthRules) {
        std::cout << rule;
    }
    std::cout << "/S";
    for (const auto rule : survivalRules) {
        std::cout << rule;
    }
    std::cout << std::endl;
    std::cout << "Iteration: " << iterations << std::endl;
    renderGrid(grid);
}

void printHelp() {
    std::cout << "\nAvailable commands:" << std::endl;
    std::cout << "  tick <n> or t <n> - calculate n generations (default: 1)" << std::endl;
    std::cout << "  auto <n> - automatically run n generations with visualization (default: 1)" << std::endl;
    std::cout << "  dump <filename> - save universe to file" << std::endl;
    std::cout << "  help - show this help" << std::endl;
    std::cout << "  exit - quit game" << std::endl;
    std::cout << "Press Enter to continue..." << std::endl;
    std::cout << std::endl;
}

Cmd ConsoleInterface::getCommand() {
    std::string cmdLine;
    std::getline(std::cin, cmdLine);
    Cmd cmd(cmdLine);
    return cmd;
}

void ConsoleInterface::printWarning(const std::string& msg) {
    warnings.push_back("[WARNING] " + msg);
}

void ConsoleInterface::printError(const std::string& msg) {
    std::cout << "[ERROR] " << msg << std::endl;
    std::cout << "Press Enter to continue..." << std::endl;
    std::cin.get(); // пауза, чтобы пользователь прочитал
}

void ConsoleInterface::flushWarnings() {
    if (!warnings.empty()) {
        std::cout << "\n— Warnings —\n";
        for (const auto& w : warnings) {
            std::cout << w << std::endl;
        }
        warnings.clear();  // очищаем буфер после показа
        std::cout << std::endl;
        std::cout << "Press Enter to continue..." << std::endl;
        std::cin.get(); // пауза, чтобы пользователь прочитал
    }
}