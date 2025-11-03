#ifndef CONFIGHANDLER_H
#define CONFIGHANDLER_H

#include <string>
#include <memory>


//базовый класс для команд конфига (предполагаемое расширение)
class ConfigCommand {
public:
    virtual ~ConfigCommand() = default;
    virtual std::string getName() const = 0;
};

class MuteCommand : public ConfigCommand {
private:
    int start{}, end{};
public:
    std::string getName() const override { return "mute"; };
    explicit MuteCommand(const std::string& attr);
    //я бы хотела чтобы в этом конструкторе была попытка преобразовать атрибуты в поля структуры.
};

class CommandFactory {
public:
    static std::unique_ptr<ConfigCommand> createCommand(const std::string& name, const std::string& attr);
};

class ConfigHandler {
private:
    std::ifstream* config;
public:
    explicit ConfigHandler(std::ifstream* in) : config(in) {};
    std::unique_ptr<ConfigCommand> getCommand() const;
};


#endif