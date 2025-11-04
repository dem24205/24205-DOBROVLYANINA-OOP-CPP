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
    auto command = factory.createCommand(cmdName, attributes);
    if (!command) throw ConfigException("Unknown command");
    return command;
}

MuteCommand::MuteCommand(const std::string &attr) {
    std::istringstream iss(attr);
    if (!(iss >> start >> end)) {
        throw ConfigException("mute: invalid arguments");
    }
    if (start < 0 || end < 0) {
        throw ConfigException("Time cannot be negative");
    }
    if (start > end) {
        throw ConfigException("Time cannot be greater than start time");
    }
    //все?
}

std::string MuteCommandCreator::getCommandName() const {
    return "name";
}

std::unique_ptr<ConfigCommand> MuteCommandCreator::createCommand(const std::string& attr) const {
    return std::make_unique<MuteCommand>(attr);
}

std::string MuteCommand::getName() const {
    return "mute";
}

void CommandFactory::registerCreator(std::unique_ptr<CommandCreator> creator) {
    creators[creator->getCommandName()] = std::move(creator);
}

CommandFactory::CommandFactory() {
    registerCreator(std::make_unique<MuteCommandCreator>());
}

std::unique_ptr<ConfigCommand> CommandFactory::createCommand(const std::string& name,
    const std::string& attr) const {
    {
        const auto it = creators.find(name);
        if (it == creators.end()) return nullptr;
        return it->second->createCommand(attr);
    }
}