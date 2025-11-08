#include "ConverterControllers.h"
#include <memory>

ConverterController::ConverterController(
    const std::string& inputName, const std::string& outputName) {
    fileHandler = new FileHandler(inputName, outputName);
    //локальный объект просто создать?
}

MixerController::MixerController(
    const std::string &inputName, const std::string &outputName)
    : ConverterController(inputName, outputName) {}

void MixerController::convert(const int start, const int end) {
    const int startInBytes = start * fileHandler->getSampleRate() * 2;
    fileHandler->seekToDataStart();
    const int inputDataSize = fileHandler->getInputDataSize();
    int dataOffset = 0;
    const int dataLength = fileHandler->getOutputDataSize();
    const MixerCreator mixerCreator;
    const auto converter = mixerCreator.createConverter();
    while (dataOffset <= dataLength) {
        if (dataOffset > inputDataSize) {
            break;
        }
        if (dataOffset >= startInBytes) {
            char* bufferIn = fileHandler->getStreamFromIn();
            char* bufferOut = fileHandler->getStreamFromOut();
            converter->convert(bufferOut, bufferIn);
            fileHandler->writeStream(bufferOut, -SECOND);
            delete[] bufferIn;
            delete[] bufferOut;
        }
        else {
            fileHandler->moveWriterPointer(SECOND);
            fileHandler->moveReaderPointer(SECOND);
        }
        dataOffset += SECOND;
    }
}

MuterController::MuterController(
    const std::string &inputName, const std::string &outputName)
    : ConverterController(inputName, outputName) {}

void MuterController::convert(int start, int end) {
    const int startInBytes = start * fileHandler->getSampleRate() * 2;
    const int endInBytes = end * fileHandler->getSampleRate() * 2;
    int dataOffset = 0;
    const int dataLength = fileHandler->getInputDataSize();
    fileHandler->seekToDataStart();
    const bool isInPlace = fileHandler->isInPlace();
    const MuterCreator muterCreator;
    const auto muter = muterCreator.createConverter();
    while (dataOffset <= dataLength) {
        bool isBlockToMute = (dataOffset >= startInBytes && dataOffset <= endInBytes);
        if (isBlockToMute) {
            char* buffer;
            if (isInPlace) {
                buffer = new char[SECOND];
            }
            else {
                buffer = fileHandler->getStreamFromIn();
            }
            muter->convert(buffer, buffer);
            fileHandler->writeStream(buffer, 0);
            delete[] buffer;
        }
        else {
            if (isInPlace) {
                fileHandler->moveWriterPointer(SECOND);
            }
            else {
                const char* buffer = fileHandler->getStreamFromIn();
                fileHandler->writeStream(buffer, 0);
            }
        }
        dataOffset += SECOND;
    }
}

std::unique_ptr<ConverterController> MixerControllerCreator::createController(
    const std::string &inputName, const std::string &outputName) {
    return std::make_unique<MixerController>(inputName, outputName);
}

std::string MixerControllerCreator::getControllerName() const {
    return "mix";
}

std::unique_ptr<ConverterController> MuterControllerCreator::createController(
    const std::string &inputName, const std::string &outputName) {
    return std::make_unique<MuterController>(inputName, outputName);
}

std::string MuterControllerCreator::getControllerName() const {
    return "mute";
}

void ControllerFactory::registerCreator(
    std::unique_ptr<ControllerCreator> creator) {
    creators[creator->getControllerName()] = std::move(creator);
}

ControllerFactory::ControllerFactory() {
    registerCreator(std::make_unique<MixerControllerCreator>());
    registerCreator(std::make_unique<MuterControllerCreator>());
}

std::unique_ptr<ConverterController> ControllerFactory::createController(
    const std::string& cmdName,
    const std::string &inputName, const std::string &outputName) {
    const auto it = creators.find(cmdName);
    if (it == creators.end()) return nullptr;
    return it->second->createController(inputName, outputName);
}