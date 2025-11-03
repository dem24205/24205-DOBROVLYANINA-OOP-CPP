#include "ConfigHandler.h"
#include "SoundProcessorException.h"
#include <memory>
#include <sstream>
#include <fstream>

std::unique_ptr<ConfigCommand> ConfigHandler::getCommand() const {
    std::string line;
    std::getline(*config, line);
    const size_t firstSpace = line.find(' ');
    std::string cmdName, attributes;
    if (firstSpace == std::string::npos) {
        cmdName = line;
        attributes = "";
    }
    else {
        cmdName = line.substr(0, firstSpace);
        attributes = line.substr(firstSpace + 1);
    }
    auto command = CommandFactory::createCommand(cmdName, attributes);
    if (!command) throw ConfigException("Unknown command");
    return command;
}

//попытка преобразовать атрибуты в поля.
//не вышло -> исключение.
MuteCommand::MuteCommand(const std::string &attr) {
    std::istringstream iss(attr);
    if (!(iss >> start >> end)) {
        throw ConfigException("mute: invalid arguments");
    }
    if (start < 0 || end < 0) {
        throw ConfigException("Time cannot be negative");
    }
    //еще что-то проверить
}


std::unique_ptr<ConfigCommand> CommandFactory::createCommand(const std::string& name, const std::string& attr) {
    if (name == "mute") {
        return std::make_unique<MuteCommand>(attr); //норм
    }
    return nullptr;
}
