#include "ConsoleParser.h"
#include "SoundProcessor.h"
#include "SoundProcessorException.h"
#include <iostream>

int main(int argc, char* argv[]) {
    RunMode mode;
    ConsoleParser parser;
    try {
        mode = parser.parse(argc, argv);
    }
    catch (ConsoleException& e) {
        std::cout << e.what() << std::endl;
        return e.getErrorCode();
    }
    if (mode == RunMode::Help) {
        SoundProcessor::printHelp();
        return 0;
    }
    try {
        auto processor = SoundProcessor(parser.getConfigFilename(), parser.getFilenames());
        processor.run();
    }
    catch (SoundProcessorException& e) {
        std::cout << e.what() << std::endl;
        return e.getErrorCode();
    }
    return 0;
}