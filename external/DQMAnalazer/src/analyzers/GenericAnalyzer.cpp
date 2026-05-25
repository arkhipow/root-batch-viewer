#include "../../include/analyzers/GenericAnalyzer.hpp"
#include <TProfile.h>
#include <TH1F.h>
#include <TF1.h>
#include <iostream>

void GenericAnalyzer::analyze(TFile* file, RunMetrics& metrics, const AnalysisConfig& config) {

    for (const auto& task : config.tasks) {
        
        if (task.type == TaskType::ProfileFit) {
            TProfile* prof = dynamic_cast<TProfile*>(file->Get(task.histPath1.c_str()));
            if (!prof) continue;

            TF1 func("dynamic_func", task.formula.c_str(), task.rangeMin, task.rangeMax);
            prof->Fit(&func, "RQNS");

            double deltaFit = task.rangeMax - task.rangeMin;
            metrics.values[task.outputName] = deltaFit * func.GetParameter(1);
            metrics.errors[task.outputName] = deltaFit * func.GetParError(1);
        }
        
        else if (task.type == TaskType::HistogramMax) {
            TH1F* hist = dynamic_cast<TH1F*>(file->Get(task.histPath1.c_str()));
            if (!hist) continue;

            metrics.values[task.outputName] = hist->GetMaximum();
            metrics.errors[task.outputName] = 0.0; // У максимума нет ошибки фита
        }
        
        // else if (task.type == TaskType::EntriesRatio) {
        //     TH1F* h1 = dynamic_cast<TH1F*>(file->Get(task.histPath1.c_str()));
        //     TH1F* h2 = dynamic_cast<TH1F*>(file->Get(task.histPath2.c_str()));
            
        //     if (!h1 || !h2 || h2->GetEntries() == 0) continue;

        //     metrics.values[task.outputName] = (double)h1->GetEntries() / (double)h2->GetEntries();
        //     metrics.errors[task.outputName] = 0.0; 
        // }

        // 4. Обработка Mean & RMS (как просил ревьюер!)
        else if (task.type == TaskType::MeanRms) {
            TH1F* hist = dynamic_cast<TH1F*>(file->Get(task.histPath1.c_str()));
            if (!hist) continue;

            // Сохраняем Mean как основное значение
            metrics.values[task.outputName + "_mean"] = hist->GetMean();
            metrics.errors[task.outputName + "_mean"] = hist->GetMeanError();

            // Сохраняем RMS как отдельную метрику
            metrics.values[task.outputName + "_rms"] = hist->GetRMS();
            metrics.errors[task.outputName + "_rms"] = hist->GetRMSError();
        }

        else if (task.type == TaskType::GaussianFit) {
            TH1F* hist = dynamic_cast<TH1F*>(file->Get(task.histPath1.c_str()));
            if (!hist) continue;

            hist->Fit("gaus", "Q");
            TF1* gaus = hist->GetFunction("gaus");
            
            if (gaus) {
                metrics.values[task.outputName + "_mean"] = gaus->GetParameter(1);
                metrics.errors[task.outputName + "_mean"] = gaus->GetParError(1);
                
                metrics.values[task.outputName + "_sigma"] = gaus->GetParameter(2);
                metrics.errors[task.outputName + "_sigma"] = gaus->GetParError(2);
            }
        }

        else if (task.type == TaskType::HWHM) {
            TH1F* hist = dynamic_cast<TH1F*>(file->Get(task.histPath1.c_str()));
            if (hist && hist->GetEntries() > 10) {
                double maxVal = hist->GetMaximum();
                double halfMax = maxVal / 2.0;

                int firstbin = hist->FindFirstBinAbove(halfMax);
                int lastbin = hist->FindLastBinAbove(halfMax);

                double leftedge = (hist->GetBinCenter(firstbin) - hist->GetBinCenter(firstbin-1)) / 
                                (hist->GetBinContent(firstbin) - hist->GetBinContent(firstbin-1)) * (halfMax - hist->GetBinContent(firstbin-1)) + hist->GetBinCenter(firstbin-1);

                double rightedge = (hist->GetBinCenter(lastbin+1) - hist->GetBinCenter(lastbin)) / 
                                (hist->GetBinContent(lastbin+1) - hist->GetBinContent(lastbin)) * (halfMax - hist->GetBinContent(lastbin)) + hist->GetBinCenter(lastbin);

                metrics.values[task.outputName] = (rightedge - leftedge) / 2.0;
            }
        }

        else if (task.type == TaskType::Z24BoardScan) {
            double globalMaxMean = 0;
            double globalMaxRMS = -1;

            for (int i = 1; i <= 70; ++i) {
                std::string boardPath = "cl/tmz/h" + std::to_string(i);
                TH1F* hBoard = dynamic_cast<TH1F*>(file->Get(boardPath.c_str()));
                if (!hBoard) continue;

                double meanTmp = std::abs(hBoard->GetMean());
                double rmsTmp = hBoard->GetRMS();
                double maxBinCenter = hBoard->GetBinCenter(hBoard->GetMaximumBin());

                if (std::abs(maxBinCenter) > 8.0) {
                    std::cout << "[Z24 ALERT] Run " << metrics.runNumber << ", Board " << i 
                            << " out of time bounds! Center: " << maxBinCenter << "\n";
                }

                if (meanTmp > globalMaxMean) globalMaxMean = meanTmp;
                if (rmsTmp > globalMaxRMS)   globalMaxRMS = rmsTmp;
            }
            metrics.values[task.outputName + "_MaxMean"] = globalMaxMean;
            metrics.values[task.outputName + "_MaxRMS"] = globalMaxRMS;
        }
        else if (task.type == TaskType::TriggerEff) {
            TH1F* hist = dynamic_cast<TH1F*>(file->Get(task.histPath1.c_str()));
            if (hist) {
                double minTuEff = 2.0; // инициализируем заведомо большим числом
                for (int b = 1; b <= 13; ++b) {
                    double val = hist->GetBinContent(b) + 3.0 * hist->GetBinError(b);
                    if (minTuEff > val) {
                        minTuEff = val;
                    }
                }
                metrics.values[task.outputName] = minTuEff;
            }
        }

    }
}