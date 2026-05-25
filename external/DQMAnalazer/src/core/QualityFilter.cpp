#include "../../include/core/QualityFilter.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>

void QualityFilter::addRule(const std::string& metricName, double min, double max) {
    rules[metricName] = {min, max};
}

std::vector<int> QualityFilter::findSuspiciousRuns(const std::vector<RunMetrics>& metricsList) {
    std::vector<int> badRuns;

    for (const auto& run : metricsList) {
        if (!run.isValid) continue;

        bool isSuspicious = false;

        for (const auto& [metricName, rule] : rules) {
            if (run.values.count(metricName)) {
                double val = run.values.at(metricName);
                
                if (val < rule.minVal || val > rule.maxVal) {
                    isSuspicious = true;
                    std::cout << "[ВНИМАНИЕ] Ран " << run.runNumber 
                              << " забракован по метрике '" << metricName 
                              << "' (Значение: " << val << ", Допустимо: " 
                              << rule.minVal << " - " << rule.maxVal << ")\n";
                    break;
                }
            }
        }

        if (isSuspicious) {
            badRuns.push_back(run.runNumber);
        }
    }

    return badRuns;
}

void QualityFilter::exportGoodRuns(const std::vector<RunMetrics>& metricsList, 
                                   const std::vector<int>& badRuns, 
                                   const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "[Error] Не удалось создать файл: " << filename << std::endl;
        return;
    }

    int goodCount = 0;
    for (const auto& run : metricsList) {
        if (!run.isValid) continue;

        if (std::find(badRuns.begin(), badRuns.end(), run.runNumber) == badRuns.end()) {
            outFile << run.runNumber << "\n";
            goodCount++;
        }
    }

    std::cout << "[Success] Золотой список из " << goodCount 
              << " хороших ранов сохранен в " << filename << std::endl;
}