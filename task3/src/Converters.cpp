#include "Converters.h"

char* Muter::convert(char* outStream, char* inStream) {
    for (int i = 0; i < SECOND; ++i) {
        outStream[i] = 0;
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

char *Mixer::convert(char *outStream, char *inStream) {
    for (int i = 0; i < SECOND; ++i) {
        outStream[i] = static_cast<char>(((outStream[i] + inStream[i]) / 2));
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