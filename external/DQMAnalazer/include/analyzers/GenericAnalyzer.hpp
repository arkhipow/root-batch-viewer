#pragma once
#include "IAnalyzer.hpp"

class GenericAnalyzer : public IAnalyzer {
public:
    void analyze(TFile* file, RunMetrics& metrics, const AnalysisConfig& config) override;
};