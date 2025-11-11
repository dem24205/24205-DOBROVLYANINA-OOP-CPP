#ifndef SOUNDPROCESSOREXCEPTION_H
#define SOUNDPROCESSOREXCEPTION_H

#include <exception>
#include <string>

class SoundProcessorException : public std::exception {
private:
    std::string message;
public:
    explicit SoundProcessorException(const std::string& message) : message(message) {};
    const char* what() const noexcept override;
    virtual int getErrorCode() const = 0;
};

class FileException : public SoundProcessorException {
private:
    int errorCode = 1;
public:
    explicit FileException(const std::string& message);
    int getErrorCode() const override;
};

class FormatException : public SoundProcessorException {
private:
    int errorCode = 2;
public:
    explicit FormatException(const std::string& message);
    int getErrorCode() const override;
};

class ConfigException : public SoundProcessorException {
private:
    int errorCode = 3;
public:
    explicit ConfigException(const std::string& message);
    int getErrorCode() const override;
};

class ConsoleException : public SoundProcessorException {
private:
    int errorCode = 4;
public:
    explicit ConsoleException(const std::string& message);
    int getErrorCode() const override;
};

#endif