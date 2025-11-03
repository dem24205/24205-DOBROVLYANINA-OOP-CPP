#ifndef SOUNDPROCESSOREXCEPTION_H
#define SOUNDPROCESSOREXCEPTION_H

#include <exception>
#include <string>

class SoundProcessorException : public std::exception {
private:
    std::string message;
public:
    explicit SoundProcessorException(const std::string& message) : message(message){};
    const char* what() const noexcept override{
        return message.c_str();
    }
    virtual int getErrorCode() const = 0;
};

class FileException : public SoundProcessorException {
private:
    int errorCode = 1; //когда исключение вспылывет наверх необходимо
                       //чтобы программа завершилась с определенным собственным кодом
public:
    explicit FileException(const std::string& message) : SoundProcessorException(message){};
    int getErrorCode() const override { return errorCode; };
};

class FormatException : public SoundProcessorException {
private:
    int errorCode = 2;
public:
    explicit FormatException(const std::string& message) : SoundProcessorException(message){};
    int getErrorCode() const override { return errorCode; }
};

class ConfigException : public SoundProcessorException {
private:
    int errorCode = 3;
public:
    explicit ConfigException(const std::string& message) : SoundProcessorException(message){};
    int getErrorCode() const override { return errorCode; }
};

// TODO: добавить исключение для конфига с кодированием
#endif