#pragma once

#include <vector>
#include <memory>
#include <string>

#include "RunMetrics.hpp"
#include "AnalysisConfig.hpp"
#include "../analyzers/IAnalyzer.hpp"

class AnalysisEngine {
private:
    AnalysisConfig config;
    std::vector<std::unique_ptr<IAnalyzer>> analyzers;

public:
    void setConfig(const AnalysisConfig& newConfig);

    void addAnalyzer(std::unique_ptr<IAnalyzer> analyzer);

    // обрабатывает один файл .root и возвращает результат
    RunMetrics processRun(int runNumber, const std::string& filePath);
};