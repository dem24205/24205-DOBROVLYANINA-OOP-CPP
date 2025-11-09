#include "Converters.h"
#include "ConverterControllers.h"

char* Muter::convert(char* outStream, char* inStream) {
    for (int i = 0; i < SECOND; i+=2) {;
        *reinterpret_cast<int16_t*>(outStream + i) = 0;
    }
    return outStream;
}

std::string Muter::getName() const {
    return "Muter";
}
std::string Muter::getDescription() const {
    return "Mutes a wave file from begin to end.";
}
std::string Muter::getAttributes() const {
    return "mute <begin[sec.]> <end[sec.]>";
}

std::unique_ptr<Converter> MuterCreator::createConverter() const {
    return std::make_unique<Muter>();
}
std::string MuterCreator::getName() const {
    return "muter";
}

char* Mixer::convert(char *outStream, char *inStream) {
    for (int i = 0; i < SECOND; i += 2) {
        const int16_t outSample = *reinterpret_cast<int16_t*>(outStream + i);
        const int16_t inSample = *reinterpret_cast<int16_t*>(inStream + i);
        const int32_t mixed = (static_cast<int32_t>(outSample) + static_cast<int32_t>(inSample)) / 2;
        *reinterpret_cast<int16_t*>(outStream + i) = static_cast<int16_t>(mixed);
    }
    return outStream;
}

std::string Mixer::getName() const {
    return "Mixer";
}

std::string Mixer::getDescription() const {
    return "Mixes two files by averaging samples from specified start time.";
}

std::string Mixer::getAttributes() const {
    return "mix $<n> <begin = 0>";
}

std::unique_ptr<Converter> MixerCreator::createConverter() const {
    return std::make_unique<Mixer>();
}

std::string MixerCreator::getName() const {
    return "mixer";
}

void ConverterFactory::registerCreator(
    std::unique_ptr<ConverterCreator> creator) {
    creators[creator->getName()] = std::move(creator);
}

ConverterFactory::ConverterFactory() {
    registerCreator(std::make_unique<MixerCreator>());
    registerCreator(std::make_unique<MuterCreator>());
}

std::vector<std::string> ConverterFactory::getAvailableConverterNames() const {
    std::vector<std::string> names;
    names.reserve(creators.size());
    for (const auto& pair : creators) {
        names.push_back(pair.first);
    }
    return names;
}

std::unique_ptr<Converter> ConverterFactory::createConverter(
    const std::string& name) const {
    auto it = creators.find(name);
    if (it != creators.end()) {
        return it->second->createConverter();
    }
    return nullptr;
}