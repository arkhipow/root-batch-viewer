#pragma once

#include "parser.hpp"
#include "widget.hpp"

#include <functional>

class Browser : public Widget {
public:
    Browser(const Rect& rect, const std::string& label);

    void render() override;

    void add(File& file);

    void setCallback(std::function<void()> callback);

    Data& getData();

private:
    std::vector<File> files;
    int currFile;
    int currData;

    std::function<void()> callback;
};