#include "ConfigHandler.h"
#include "SoundProcessorException.h"
#include <memory>
#include <sstream>
#include <fstream>

std::unique_ptr<ConfigCommand> ConfigHandler::getCommand() const {
    std::string line;
    std::getline(*config, line);
    std::string cmdName, attributes;
    const size_t firstSpace = line.find(' ');
    if (line[0] == '#' || line.empty()) {
        cmdName = "comment";
        attributes = "";
    }
    else {
        if (firstSpace == std::string::npos) {
            throw ConfigException("Invalid command line");
        }
        cmdName = line.substr(0, firstSpace);
        attributes = line.substr(firstSpace + 1);
    }

    auto command = factory.createCommand(cmdName, attributes);
    if (!command) throw ConfigException("Unknown command");
    return command;
}

MixCommand::MixCommand(const std::string &attr) {
    const size_t refPos = attr.find('$');
    if (refPos == std::string::npos) {
        throw ConfigException("Incorrect attribute format");
    }
    start = 0;
    std::istringstream iss(attr.substr(refPos + 1));
    if (!(iss >> fileIdx)) {
        throw ConfigException("mix: invalid parameters");
    }
    iss >> start;
    if (fileIdx < 1) {
        throw ConfigException("mix: invalid parameters");
    }
    if (start < 0) {
        throw ConfigException("mix: invalid parameters");
    }
}

std::string CommentCommand::getName() const {
    return "comment";
}

std::string MixCommand::getName() const {
    return "mix";
}

std::string CommentCreator::getCommandName() const {
    return "comment";
}

std::unique_ptr<ConfigCommand> CommentCreator::createCommand(const std::string& attr) const {
    return std::make_unique<CommentCommand>();
}

std::unique_ptr<ConfigCommand> MixCommandCreator::createCommand(const std::string& attr) const {
    return std::make_unique<MixCommand>(attr);
}

std::string MixCommandCreator::getCommandName() const {
    return "mix";
}


MuteCommand::MuteCommand(const std::string &attr) {
    std::istringstream iss(attr);
    if (!(iss >> start >> end)) {
        throw ConfigException("mute: invalid parameters");
    }
    if (start < 0 || end < 0) {
        throw ConfigException("mute: invalid parameters");
    }
    if (start > end) {
        throw ConfigException("mute: invalid parameters");
    }
}

std::string MuteCommandCreator::getCommandName() const {
    return "mute";
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
    registerCreator(std::make_unique<MixCommandCreator>());
    registerCreator(std::make_unique<CommentCreator>());
}

std::unique_ptr<ConfigCommand> CommandFactory::createCommand(const std::string& name,
    const std::string& attr) const {
    {
        const auto it = creators.find(name);
        if (it == creators.end()) return nullptr;
        return it->second->createCommand(attr);
    }
}