#include "plot.hpp"
#include <algorithm>

Graph::Graph(const Rect& rect, const std::string& label)
    : Widget(rect, label) {}

void Graph::reset() {
    ImPlot::SetNextAxesToFit();
}

Plot::Plot(const Rect& rect, const std::string& label)
    : Graph(rect, label), logX(false), logY(false) {}

void Plot::render() {
    Rect rect = getRectPixels();
    ImGui::SetCursorPos(ImVec2(rect.x, rect.y));

    if (ImPlot::BeginPlot(label.c_str(), ImVec2(rect.width, rect.height))) {
        if (logX) {
            ImPlot::SetupAxisScale(ImAxis_X1, ImPlotScale_Log10);
        }

        if (logY) {
            ImPlot::SetupAxisScale(ImAxis_Y1, ImPlotScale_Log10);
        }

        ImPlotSpec spec;
        spec.LineWeight = 2;
        spec.Marker = ImPlotMarker_Circle;

        ImPlot::PlotLine(("##" + label).c_str(), x.data(), y.data(), x.size(), spec);
        ImPlot::EndPlot();
    }
}

void Plot::setData(const std::vector<float>& x, const std::vector<float>& y) {
    this->x = x;
    this->y = y;
}

bool Plot::getLogX() const noexcept {
    return logX;
}

bool Plot::getLogY() const noexcept {
    return logY;
}

void Plot::setLogX(bool logX) noexcept {
    this->logX = logX;
}

void Plot::setLogY(bool logY) noexcept {
    this->logY = logY;
}

Histogram::Histogram(const Rect& rect, const std::string& label)
    : Graph(rect, label), bins(-2) {}

void Histogram::render() {
    Rect rect = getRectPixels();
    ImGui::SetCursorPos(ImVec2(rect.x, rect.y));

    if (ImPlot::BeginPlot(label.c_str(), ImVec2(rect.width, rect.height))) {
        ImPlotSpec spec;
        spec.FillAlpha = 0.5;

        // ImPlotRange range(*(std::min_element)(x.begin(), x.end()), *(std::max_element)(x.begin(), x.end()));

        ImPlot::PlotHistogram(("##" + label).c_str(), x.data(), x.size(), bins, 1, ImPlotRange(), spec);

        ImPlot::EndPlot();
    }
}

void Histogram::setData(const std::vector<float>& x, const std::vector<float>& y) {
    this->x = x;
    this->y = y;
}

void Histogram::setBins(int bins) noexcept {
    this->bins = bins;
}