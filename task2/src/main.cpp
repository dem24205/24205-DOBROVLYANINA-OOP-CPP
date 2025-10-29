#include "GameConfig.h"
#include "LifeGame.h"
#include "LifeParser.h"
#include <windows.h>

int main(int argc, char* argv[]) {
    SetConsoleOutputCP(65001);
    std::string filename;
    if (argc > 1) {
        filename = argv[1];
    }
    LifeParser parser(filename);
    GameConfig config = parser.handleParsing(argc, argv);
    LifeGame game(config);
    game.start();
    return 0;
}

//TODO: дописать тесты для нового парсера
