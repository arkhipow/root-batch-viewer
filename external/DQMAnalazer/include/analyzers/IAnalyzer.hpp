#pragma once
#include <TFile.h>
#include "../core/RunMetrics.hpp"
#include "../core/AnalysisConfig.hpp"

class IAnalyzer {
public:
    virtual ~IAnalyzer() = default;

    virtual void analyze(TFile* file, RunMetrics& metrics, const AnalysisConfig& config) = 0;
};