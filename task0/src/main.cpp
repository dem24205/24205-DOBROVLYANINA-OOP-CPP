#include "FileReader.h"
#include "CSVFileWriter.h"
#include "TextParser.h"
#include "WordFrequencyAnalyzer.h"
#include <iomanip>
#include <iostream>
#include <sstream>
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
        analyzer.addWords(words);
    }

    CSVFileWriter writer(argv[2]);
    if (!writer.isOpen()) {
        std::cerr << "Error: Cannot open output file: " << argv[2] << "\n";
        return 1;
    }

    std::vector<WordStat> stats = analyzer.getFrequencyStat();

    writer.write({"word", "count", "percent"});
    for (const auto& wordStat : stats) {
        std::ostringstream freqStream;
        freqStream << std::fixed << std::setprecision(3) << wordStat.getFrequency();
        std::vector<std::string> row = {
            wordStat.getWord(),
            std::to_string(wordStat.getCount()),
            freqStream.str()
        };
        writer.write(row);
    }
}