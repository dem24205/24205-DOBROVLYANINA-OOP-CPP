#include "FileReader.h"
#include "FileWriter.h"
#include "TextParser.h"
#include "WordFrequencyAnalyzer.h"
#include <string>

int main(int argc, char* argv[]) {
    FileReader reader(argv[1]);
    if (!reader.isOpen()) {
        return 1;
    }
    TextParser parser;
    while (!reader.isEOF()) {
        const std::string line = reader.getLine();
        parser.parseLine(line);
    }
    WordFrequencyAnalyzer analyzer;
    analyzer.analyzeWordFrequency(parser.getWordCount(), parser.getTotalWords());
    FileWriter writer(argv[2]);
    if (!writer.isOpen()) {
        return 1;
    }
    writer.writeToFile(analyzer.getFrequencyStat());
    return 0;
}