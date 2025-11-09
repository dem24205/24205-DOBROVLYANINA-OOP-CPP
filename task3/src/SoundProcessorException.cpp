#include "SoundProcessorException.h"

const char* SoundProcessorException::what() const noexcept {
    return message.c_str();
}

FileException::FileException(const std::string& message)
    : SoundProcessorException(message) {}

int FileException::getErrorCode() const {
    return errorCode;
}

FormatException::FormatException(const std::string& message)
    : SoundProcessorException(message) {}

int FormatException::getErrorCode() const {
    return errorCode;
}

ConfigException::ConfigException(const std::string& message)
    : SoundProcessorException(message) {}

int ConfigException::getErrorCode() const {
    return errorCode;
}