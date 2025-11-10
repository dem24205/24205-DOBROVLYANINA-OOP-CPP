#ifndef CONVERTERS_H
#define CONVERTERS_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Converter {
protected:
    const int SECOND = 44100 * 2;
public:
    virtual ~Converter() = default;
    virtual char* convert (char* outStream, char* inStream, int blocks) = 0;
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual std::string getAttributes() const = 0;

};

class Muter : public Converter {
    char* convert (char* outStream, char* inStream, int blocks) override;
    std::string getName() const override;
    std::string getDescription() const override;
    std::string getAttributes() const override;

};

class Mixer : public Converter {
    char* convert (char* outStream, char* inStream, int blocks) override;
    std::string getName() const override;
    std::string getDescription() const override;
    std::string getAttributes() const override;
};

class Reverser : public Converter {
    char* convert (char* outStream, char* inStream, int blocks) override;
    std::string getName() const override;
    std::string getDescription() const override;
    std::string getAttributes() const override;
};

class ConverterCreator {
public:
    virtual ~ConverterCreator() = default;
    virtual std::string getName() const = 0;
    virtual std::unique_ptr<Converter> createConverter() const = 0;
};

class MuterCreator : public ConverterCreator {
public:
    std::string getName() const override;
    std::unique_ptr<Converter> createConverter() const override;
};

class MixerCreator : public ConverterCreator {
public :
    std::unique_ptr<Converter> createConverter() const override;
    std::string getName() const override;
};

class ReverserCreator : public ConverterCreator {
public:
    std::unique_ptr<Converter> createConverter() const override;
    std::string getName() const override;
};

class ConverterFactory {
private:
    void registerCreator(std::unique_ptr<ConverterCreator> creator);
protected:
    std::unordered_map<std::string, std::unique_ptr<ConverterCreator>> creators;
public:
    std::vector<std::string> getAvailableConverterNames() const;
    std::unique_ptr<Converter> createConverter(const std::string& name) const;
    ConverterFactory();
};

#endif