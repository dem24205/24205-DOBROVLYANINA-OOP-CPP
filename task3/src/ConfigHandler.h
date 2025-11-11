#ifndef CONFIGHANDLER_H
#define CONFIGHANDLER_H

#include <memory>
#include <string>
#include <unordered_map>

class ConfigCommand {
protected:
    int start{};
    int end{};
    int inputFileIdx = 1;
public:
    virtual ~ConfigCommand() = default;
    virtual std::string getName() const = 0;
    virtual int getStart() const  = 0;
    virtual int getEnd() const  = 0;
    virtual int getInputFileIdx() const = 0;
};

class ReverseCommand : public ConfigCommand {
public:
    explicit ReverseCommand(const std::string& attr);
    std::string getName() const override;
    int getStart() const override;
    int getEnd() const override;
    int getInputFileIdx() const override;

};

class CommentCommand : public ConfigCommand {
public:
    std::string getName() const override;
    int getStart() const override;
    int getEnd() const override;
    int getInputFileIdx() const override;
};

class MuteCommand : public ConfigCommand {
public:
    explicit MuteCommand(const std::string& attr);
    std::string getName() const override;
    int getStart() const override;
    int getEnd() const override;
    int getInputFileIdx() const override;
};

class MixCommand : public ConfigCommand {
public:
    explicit MixCommand(const std::string& attr);
    std::string getName() const override;
    int getStart() const override;
    int getEnd() const override;
    int getInputFileIdx() const override;
};

class CommandCreator {
public:
    virtual ~CommandCreator() = default;
    virtual std::string getCommandName() const = 0;
    virtual std::unique_ptr<ConfigCommand> createCommand(const std::string& attr) const = 0;
};

class CommentCreator : public CommandCreator {
public:
    std::string getCommandName() const override;
    std::unique_ptr<ConfigCommand> createCommand(const std::string& attr) const override;
};

class MuteCommandCreator : public CommandCreator {
public:
    std::string getCommandName() const override;
    std::unique_ptr<ConfigCommand> createCommand(const std::string& attr) const override;
};

class ReverseCommandCreator : public CommandCreator {
public:
    std::string getCommandName() const override;
    std::unique_ptr<ConfigCommand> createCommand(const std::string& attr) const override;
};

class MixCommandCreator : public CommandCreator {
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