#include "FileReader.h"
#include "FileWriter.h"
#include "TextParser.h"
#include "WordFrequencyAnalyzer.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>\n";
        return 1;
    }

    FileReader reader(argv[1]);
    if (!reader.isOpen()) {
        std::cerr << "Error: Cannot open input file: " << argv[1] << "\n";
        return 1;
    }

    TextParser parser;
    WordFrequencyAnalyzer analyzer;

    while (!reader.isEOF()) {
        const std::string line = reader.getLine();
        const std::vector<std::string> words = parser.parseLine(line);
        analyzer.addWord(words);
    }

    FileWriter writer(argv[2]);
    if (!writer.isOpen()) {
        std::cerr << "Error: Cannot open output file: " << argv[2] << "\n";
        return 1;
    }

    writer.writeToFile(analyzer.getFrequencyStat());
    return 0;
}