#include "ConfigHandler.h"
#include "ConverterControllers.h"
#include "Converters.h"
#include "SoundProcessor.h"
#include "SoundProcessorException.h"
#include <iostream>

static bool isInPlace(const std::unique_ptr<ConfigCommand>& cmd) {
    return cmd->getInputFileIdx() == 1;
}

static bool isNotToConvert(const std::unique_ptr<ConfigCommand>& cmd) {
    return cmd->getName() == "comment";
}

void SoundProcessor::run() {
    config.open(configFilename);
    if (!config.is_open()) {
        throw FileException("Cannot open file " + configFilename);
    }
    const ConfigHandler handler(&config);
    std::string prevFilename = filenames[1];
    const std::string outputFilename = filenames[0];
    initializeOutput(outputFilename, prevFilename);
    while (!config.eof()) {
        auto command = handler.getCommand();
        if (isNotToConvert(command)) continue;
        ControllerFactory factory;
        std::unique_ptr<ConverterController> converterController;
        //TODO: check filenames[command->getInputiIdx()]
        if (isInPlace(command)) {
            converterController = factory.createController(command->getName(), prevFilename, outputFilename);
        }
        else {
            converterController = factory.createController(command->getName(), filenames[command->getInputFileIdx()], outputFilename);
        }
        converterController->convert(command->getStart(), command->getEnd());
        prevFilename = outputFilename;
    }
    config.close();
}

void SoundProcessor::initializeOutput(const std::string& outputName, const std::string& firstInput) {
    remove(outputName.c_str());
    MuterControllerCreator creator;
    const auto muterController = creator.createController(firstInput, outputName);
    muterController->convert(0, 0);
}

void SoundProcessor::printHelp() {
    const ConverterFactory factory;
    const auto converterNames = factory.getAvailableConverterNames();
    std::cout << "Available converters:" << std::endl;
    for (const auto& name : converterNames) {
        const auto converter = factory.createConverter(name);
        std::cout << converter->getName() << std::endl;
        std::cout << converter->getAttributes() << std::endl;
        std::cout << converter->getDescription() << std::endl;
        std::cout << std::endl;
    }
}