#include "../../include/core/Engine.hpp"
#include <TFile.h>
#include <iostream>

void AnalysisEngine::setConfig(const AnalysisConfig& newConfig) {
    config = newConfig;
}

void AnalysisEngine::addAnalyzer(std::unique_ptr<IAnalyzer> analyzer) {
    analyzers.push_back(std::move(analyzer));
}

RunMetrics AnalysisEngine::processRun(int runNumber, const std::string& filePath) {
    RunMetrics metrics;
    metrics.runNumber = runNumber;
    metrics.isValid = false; // по умолчанию файл битый

    std::unique_ptr<TFile> file(TFile::Open(filePath.c_str(), "READ"));
    
    if (!file || file->IsZombie()) {
        std::cerr << "[Error] Не могу открыть файл: " << filePath << std::endl;
        return metrics;
    }

    metrics.isValid = true;

    for (const auto& analyzer : analyzers) {
        analyzer->analyze(file.get(), metrics, config);
    }

    return metrics;
}