#pragma once
#include <string>
#include <vector>
#include "../core/RunMetrics.hpp"

class ResultWriter {
public:
    bool writeResults(const std::string& outputFilePath, const std::vector<RunMetrics>& allMetrics);
};