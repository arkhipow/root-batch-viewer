#pragma once
#include <vector>
#include <string>


class RunListParser {
public:
    //возвращает массив номеров ранов
    static std::vector<int> parseFromFile(const std::string& filename);
};