#include "../../include/io/ResultWriter.hpp"
#include <TFile.h>
#include <TTree.h>
#include <iostream>

bool ResultWriter::writeResults(const std::string& outputFilePath, const std::vector<RunMetrics>& allMetrics) {
    if (allMetrics.empty()) {
        std::cerr << "[Warning] Нет данных для записи!" << std::endl;
        return false;
    }

    TFile* outFile = TFile::Open(outputFilePath.c_str(), "RECREATE");
    if (!outFile || outFile->IsZombie()) {
        std::cerr << "[Error] Не удалось создать файл для записи: " << outputFilePath << std::endl;
        return false;
    }

    TTree* tree = new TTree("SummaryTree", "Результирующие метрики DQM по ранам");

    int runNumBuffer = 0;
    std::map<std::string, double> valueBuffers;
    std::map<std::string, double> errorBuffers;

    tree->Branch("runNumber", &runNumBuffer, "runNumber/I");

    const RunMetrics* sample = nullptr;
    for (const auto& m : allMetrics) {
        if (m.isValid) {
            sample = &m;
            break;
        }
    }

    if (sample) {
        for (const auto& pair : sample->values) {
            const std::string& name = pair.first;
            valueBuffers[name] = 0.0;
            errorBuffers[name] = 0.0;

            tree->Branch(name.c_str(), &valueBuffers[name], (name + "/D").c_str());
            tree->Branch((name + "_err").c_str(), &errorBuffers[name], (name + "_err/D").c_str());
        }
    }

    for (const auto& metrics : allMetrics) {
        if (!metrics.isValid) continue;

        runNumBuffer = metrics.runNumber;

        for (const auto& pair : metrics.values) {
            valueBuffers[pair.first] = pair.second;
        }
        for (const auto& pair : metrics.errors) {
            errorBuffers[pair.first] = pair.second;
        }

        tree->Fill();
    }

    tree->Write();
    outFile->Close();
    delete outFile;

    std::cout << "[Success] Результаты успешно сохранены в " << outputFilePath << std::endl;
    return true;
}