#include "../../include/io/RunListParser.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<int> RunListParser::parseFromFile(const std::string& filename) {
    std::vector<int> runs;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "[Error] Не удалось открыть список ранов (recolist): " << filename << std::endl;
        return runs;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        std::string firstWord;
        
        if (iss >> firstWord) {
            try {
                int runNumber = std::stoi(firstWord);
                runs.push_back(runNumber);
            } catch (const std::exception&) {
                continue;
            }
        }
    }

    std::cout << "[Info] Прочитано ранов из " << filename << ": " << runs.size() << std::endl;
    return runs;
}