#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <TFile.h>
#include <TH1F.h>

#include "analyzers/GenericAnalyzer.hpp"
#include "core/AnalysisConfig.hpp"
#include "core/Engine.hpp"
#include "core/QualityFilter.hpp"
#include "core/RunMetrics.hpp"
#include "io/ResultWriter.hpp"

#include <iostream>

#include <TROOT.h>
#include <TList.h>

class DQMAlgoProcessor {
private:
    AnalysisEngine m_engine;
    QualityFilter  m_filter;
    ResultWriter   m_writer;

public:
    static TaskType parseTaskType(const std::string& algoName) {
        if (algoName == "Profile Fit")        return TaskType::ProfileFit;
        if (algoName == "Mean Rms")           return TaskType::MeanRms;
        if (algoName == "Gaussian Fit")       return TaskType::GaussianFit;
        if (algoName == "Histogram Subtract") return TaskType::HistogramSubtract;
        if (algoName == "Integral")           return TaskType::Integral;
        if (algoName == "HWHM")               return TaskType::HWHM;
        if (algoName == "Trigger Eff")        return TaskType::TriggerEff;
        if (algoName == "Z24 Board Scan")     return TaskType::Z24BoardScan;
        return TaskType::HistogramMax;
    }

    static std::string taskTypeToString(TaskType type) {
        switch (type) {
            case TaskType::ProfileFit:        return "Profile Fit";
            case TaskType::MeanRms:           return "Mean Rms";
            case TaskType::GaussianFit:       return "Gaussian Fit";
            case TaskType::HistogramSubtract: return "Histogram Subtract";
            case TaskType::Integral:          return "Integral";
            case TaskType::HWHM:              return "HWHM";
            case TaskType::TriggerEff:        return "Trigger Eff";
            case TaskType::Z24BoardScan:      return "Z24 Board Scan";
            case TaskType::HistogramMax:      return "Histogram Max";
            default:                          return "Unknown Algorithm";
        }
    }

    DQMAlgoProcessor() {
        m_engine.addAnalyzer(std::make_unique<GenericAnalyzer>());
    }

    // ==============================================================================
    // Run One

    bool processSingleHistogram(
        const std::string& algoName,
        const std::string& histName,
        const std::vector<float>& currentX,
        const std::vector<float>& currentY,
        double filterMin, double filterMax,
        float& outResultX, float& outResultY
    ) {
        if (currentX.empty() || currentY.empty() || histName.empty()) {
            std::cout << "[DQM Error] Invalid input data!" << std::endl;
            return false;
        }

        std::vector<float> filteredX;
        std::vector<float> filteredY;
        TaskType currentType = parseTaskType(algoName);

        for (size_t i = 0; i < currentX.size(); ++i) {
            if (currentX[i] >= filterMin && currentX[i] <= filterMax) {
                filteredX.push_back(currentX[i]);
                filteredY.push_back(currentY[i]);
            }
        }

        if (filteredX.empty()) {
            std::cout << "[DQM Safe Guard] Error: Range [" << filterMin << " ; " << filterMax << "] empty for histogram " << histName << "!" << std::endl;
            return false; 
        }

        TFile* memFile = TFile::Open("temp/temp.root", "RECREATE");
        if (!memFile || memFile->IsZombie()) {
            std::cout << "[DQM Error] Failed to create temporary root file!";
            return false;
        }

        float binWidth = 1.0f;
        if (filteredX.size() > 1) {
            binWidth = filteredX[1] - filteredX[0];
        }

        float lowEdge = filteredX.front() - (binWidth / 2.0f);
        float highEdge = filteredX.back() + (binWidth / 2.0f);
        int nBins = static_cast<int>(filteredX.size());

        if (nBins <= 0) {
            std::cout << "[DQM Error] Histogram has no columns.!";
            memFile->Close();
            delete memFile;
            return false;
        }

        TH1F* rootHist = new TH1F(histName.c_str(), histName.c_str(), nBins, lowEdge, highEdge);
        
        for (int i = 0; i < nBins; ++i) {
            rootHist->SetBinContent(i + 1, filteredY[i]);
        }

        memFile->Write();

        AnalysisConfig singleConfig;
        AnalyzerTask task;
        task.type = currentType; 
        task.histPath1 = histName;           
        task.outputName = "single_ui_metric"; 
        task.rangeMin = lowEdge;
        task.rangeMax = highEdge;
        singleConfig.tasks.push_back(task);

        RunMetrics metrics;
        metrics.runNumber = 1;

        GenericAnalyzer analyzer;
        analyzer.analyze(memFile, metrics, singleConfig);

        memFile->Close();
        delete memFile;

        if (metrics.values.count("single_ui_metric")) {
            outResultX = 1.0f; 
            outResultY = static_cast<float>(metrics.values["single_ui_metric"]); 
            return true;
        }

        if (!metrics.values.empty()) {
            std::string fallbackKey = "";

            for (auto const& [key, val] : metrics.values) {
                std::cout << "    -> Key: \"" << key << "\" | Value: " << val << std::endl;
                fallbackKey = key; 
            }

            if (!fallbackKey.empty()) {
                outResultX = 1.0f;
                outResultY = static_cast<float>(metrics.values[fallbackKey]);
                std::cout << ">>> [DQM Debug] Fallback key: \"" << fallbackKey << "\"\n" << std::endl;
                return true;
            }
        }

        std::cout << "[DQM Error] Results map metrics.values is empty!" << std::endl;
        
        return false;
    }
};