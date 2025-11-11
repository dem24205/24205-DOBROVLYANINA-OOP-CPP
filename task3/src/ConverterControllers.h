#ifndef CONVERTERCONTROLLERS_H
#define CONVERTERCONTROLLERS_H

#include <string>
#include <unordered_map>
#include <memory>
#include "FileHandler.h"

class ConverterController {
protected:
    const int SECOND = 44100 * 2;
    FileHandler fileHandler;

public:
    ConverterController(const std::string &inputName, const std::string& outputName);
    virtual ~ConverterController() = default;
    virtual void convert(int start, int end) = 0;
};

class MuterController : public ConverterController {
public:
    using ConverterController::ConverterController;
    void convert(int start, int end) override;
};

class MixerController : public ConverterController {
public:
    using ConverterController::ConverterController;
    void convert(int start, int end) override;
};

class ReverserController : public ConverterController {
public:
    using ConverterController::ConverterController;
    void convert(int start, int end) override;
};

class ControllerCreator {
public:
    virtual std::unique_ptr<ConverterController> createController(const std::string &inputName, const std::string& outputName) = 0;
    virtual std::string getControllerName() const = 0;
    virtual ~ControllerCreator() = default;
};

class MixerControllerCreator : public ControllerCreator {
public:
    std::unique_ptr<ConverterController> createController(const std::string &inputName, const std::string& outputName) override;
    std::string getControllerName() const override;
};

class MuterControllerCreator : public ControllerCreator {
public:
    std::unique_ptr<ConverterController> createController(const std::string &inputName, const std::string& outputName) override;
    std::string getControllerName() const override;
};

class ReverserControllerCreator : public ControllerCreator {
public:
    std::unique_ptr<ConverterController> createController(const std::string &inputName, const std::string& outputName) override;
    std::string getControllerName() const override;
};

class ControllerFactory {
private:
    std::unordered_map<std::string, std::unique_ptr<ControllerCreator>> creators;
    void registerCreator(std::unique_ptr<ControllerCreator> creator);
public:
    ControllerFactory();
    std::unique_ptr<ConverterController> createController(const std::string& cmdName, const std::string &inputName, const std::string& outputName);
};

#endif