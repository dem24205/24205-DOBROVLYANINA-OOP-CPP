#include "ConverterControllers.h"
#include "Converters.h"
#include <cstring>
#include <memory>

ConverterController::ConverterController(
    const std::string& inputName, const std::string& outputName)
    : fileHandler(inputName, outputName) {}

//controllers
void MixerController::convert(const int start, const int end) {
    const int startInBytes = start * fileHandler.getSampleRate() * 2;
    fileHandler.seekToDataStart();
    const int inputDataSize = fileHandler.getInputDataSize();
    int dataOffset = 0;
    const int dataLength = fileHandler.getOutputDataSize();
    const MixerCreator mixerCreator;
    const auto mixer = mixerCreator.createConverter();
    while (dataOffset <= dataLength) {
        if (dataOffset > inputDataSize) {
            break;
        }
        if (dataOffset >= startInBytes) {
            auto bufferIn = fileHandler.getStreamFromIn();
            auto bufferOut = fileHandler.getStreamFromOut();
            mixer->convert(bufferOut.data(), bufferIn.data(), 1);
            fileHandler.writeStream(bufferOut.data(), -SECOND, 1);
        } else {
            fileHandler.moveWriterPointer(SECOND);
            fileHandler.moveReaderPointer(SECOND);
        }
        dataOffset += SECOND;
    }
}

void MuterController::convert(const int start, const int end) {
    const int startInBytes = start * fileHandler.getSampleRate() * 2;
    const int endInBytes = end * fileHandler.getSampleRate() * 2;
    int dataOffset = 0;
    const int dataLength = fileHandler.getInputDataSize();
    fileHandler.seekToDataStart();
    const bool isInPlace = fileHandler.isInPlace();
    const MuterCreator muterCreator;
    const auto muter = muterCreator.createConverter();
    while (dataOffset <= dataLength) {
        bool isBlockToMute = (dataOffset >= startInBytes && dataOffset <= endInBytes);
        if (isBlockToMute) {
            std::vector<char> buffer;
            if (isInPlace) {
                buffer = std::vector<char>(SECOND, 0);
            } else {
                buffer = fileHandler.getStreamFromIn();
            }
            muter->convert(buffer.data(), buffer.data(), 1);
            fileHandler.writeStream(buffer.data(), 0, 1);
        } else {
            if (isInPlace) {
                fileHandler.moveWriterPointer(SECOND);
            } else {
                auto buffer = fileHandler.getStreamFromIn();
                fileHandler.writeStream(buffer.data(), 0, 1);
            }
        }
        dataOffset += SECOND;
    }
}

void ReverserController::convert(const int start, const int end) {
    const int startInBytes = start * fileHandler.getSampleRate() * 2;
    const int endInBytes = end * fileHandler.getSampleRate() * 2;
    const int sizeInBytes = endInBytes - startInBytes;
    const int blocks = end - start;
    std::vector<char>dataBlockToConvert(sizeInBytes);
    fileHandler.seekToPointer(startInBytes);
    for (int i = 0; i < sizeInBytes; i+= SECOND) {
        std::vector<char> buffer = fileHandler.getStreamFromOut();
        std::memcpy(dataBlockToConvert.data() + i, buffer.data(), SECOND);
    }
    const ReverserCreator creator;
    const auto reverser = creator.createConverter();
    reverser->convert(dataBlockToConvert.data(), dataBlockToConvert.data(), blocks);
    fileHandler.seekToPointer(startInBytes);
    fileHandler.writeStream(dataBlockToConvert.data(), 0, blocks);
}

std::unique_ptr<ConverterController> MixerControllerCreator::createController(
    const std::string &inputName, const std::string &outputName) {
    return std::make_unique<MixerController>(inputName, outputName);
}

std::string MixerControllerCreator::getControllerName() const {
    return "mix";
}

//creators
std::unique_ptr<ConverterController> ReverserControllerCreator::createController(
    const std::string &inputName, const std::string &outputName) {
    return std::make_unique<ReverserController>(inputName, outputName);
}

std::string ReverserControllerCreator::getControllerName() const {
    return "reverse";
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
    registerCreator(std::make_unique<ReverserControllerCreator>());
}

std::unique_ptr<ConverterController> ControllerFactory::createController(
    const std::string& cmdName,
    const std::string &inputName, const std::string &outputName) {
    const auto it = creators.find(cmdName);
    if (it == creators.end()) return nullptr;
    return it->second->createController(inputName, outputName);
}