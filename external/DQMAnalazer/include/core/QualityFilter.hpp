#pragma once
#include <vector>
#include <string>
#include <map>
#include "RunMetrics.hpp"

struct FilterRule {
    double minVal;
    double maxVal;
};

class QualityFilter {
private:
    std::map<std::string, FilterRule> rules;

public:
    void addRule(const std::string& metricName, double min, double max);

    // проверяет все раны и возвращает список плохих
    std::vector<int> findSuspiciousRuns(const std::vector<RunMetrics>& metricsList);

    // сохраняет хорошие раны в текстовый файл
    void exportGoodRuns(const std::vector<RunMetrics>& metricsList, 
                        const std::vector<int>& badRuns, 
                        const std::string& filename);
};