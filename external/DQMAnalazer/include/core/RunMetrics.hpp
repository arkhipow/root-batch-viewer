#pragma once
#include <map>
#include <string>


struct RunMetrics {
    int runNumber = -1;
    bool isValid = false; 

    std::map<std::string, double> values;
    
    std::map<std::string, double> errors;
};