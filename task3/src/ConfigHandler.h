#ifndef CONFIGHANDLER_H
#define CONFIGHANDLER_H

#include <string>
#include <memory>
#include <unordered_map>

//предполагаемое расширение
class ConfigCommand {
public:
    virtual ~ConfigCommand() = default;
    virtual std::string getName() const = 0;
};

class MuteCommand : public ConfigCommand {
private:
    int start{}, end{};
public:
    std::string getName() const override;
    explicit MuteCommand(const std::string& attr);
};

class CommandCreator {
public:
    virtual ~CommandCreator() = default;
    virtual std::string getCommandName() const = 0;
    virtual std::unique_ptr<ConfigCommand> createCommand(const std::string& attr) const = 0;
};

class MuteCommandCreator : public CommandCreator {
public:
    std::string getCommandName() const override;
    std::unique_ptr<ConfigCommand> createCommand(const std::string& attr) const override;
};

class CommandFactory {
private:
    std::unordered_map<std::string, std::unique_ptr<CommandCreator>> creators;
    void registerCreator(std::unique_ptr<CommandCreator> creator);
public:

    CommandFactory();
    std::unique_ptr<ConfigCommand> createCommand(const std::string& name,
    const std::string& attr) const;
};

class ConfigHandler {
private:
    CommandFactory factory;
    std::ifstream* config;
public:
    explicit ConfigHandler(std::ifstream* in) : config(in) {};
    std::unique_ptr<ConfigCommand> getCommand() const;
};


#endif