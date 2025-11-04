#ifndef CONVERTERS_H
#define CONVERTERS_H

#include <memory>
#include <string>

class Converter {
protected:
    const int SECOND = 44100 * 2;
public:
    virtual ~Converter() = default;
    virtual char* convert (char* outStream, char* inStream) = 0;
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual std::string getAttributes() const = 0;

};

class Muter : public Converter {
    //по дефолту inStream = nullptr
    char* convert (char* outStream, char* inStream) override;
    std::string getName() const override;
    std::string getDescription() const override;
    std::string getAttributes() const override;

};

class Mixer : public Converter {
    char* convert (char* outStream, char* inStream) override;
    std::string getName() const override;
    std::string getDescription() const override;
    std::string getAttributes() const override;
};

class ConverterCreator {
public:
    virtual ~ConverterCreator() = default;
    virtual std::unique_ptr<Converter> createConverter() const = 0;
};

class MuterCreator : public ConverterCreator {
public:
    std::unique_ptr<Converter> createConverter() const override;
};

class MixerCreator : public ConverterCreator {
public :
    std::unique_ptr<Converter> createConverter() const override;
};

#endif