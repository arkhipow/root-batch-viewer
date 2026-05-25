#pragma once
#include <string>
#include <vector>


enum class TaskType {
    ProfileFit,        // Фит TProfile
    HistogramMax,      // Поиск максимума TH1F
    MeanRms,           // Базовые Mean и RMS
    GaussianFit,       // Фит Гауссом для поиска пика энергии (EtotMeanF)
    HistogramSubtract, // Вычитание гистограмм (как для cbebe и cbebe27)
    Integral,          // Интеграл по бинам (AddPhotBebe)
    HWHM,              // ручной расчет полуширины на полувысоте (dphi/dtheta)
    TriggerEff,        // Хитрый поиск минимума TU эффективности (Мин + 3*Ошибка)
    Z24BoardScan       // Сканирование 70 плат тайминга Z24 калориметра
};

struct AnalyzerTask {
    TaskType type;
    std::string histPath1;
    std::string histPath2 = "";  //нужен только для EntriesRatio
    std::string formula = "";    //нужна только для ProfileFit
    double rangeMin = 0.0; 
    double rangeMax = 0.0;   
    std::string outputName; 
};

struct AnalysisConfig {
    std::string inputFolder = "./";
    
    std::vector<AnalyzerTask> tasks;
};