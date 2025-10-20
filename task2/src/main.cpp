#include "GameConfig.h"
#include "GameOfLife.h"
#include "LifeParser.h"
#include <windows.h>

int main(int argc, char* argv[]) {
    SetConsoleOutputCP(65001);
    LifeParser parser(argc, argv);
    GameConfig config;
    parser.handleParsing(config, argc, argv);
    GameOfLife game(config);
    game.start();
    return 0;
}

//test.life
//test.life -i=100 -o=out_universe