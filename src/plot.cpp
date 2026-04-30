#include "plot.hpp"

Plot::Plot(const Rect& rect, const std::string& label)
    : Widget(rect, label), logX(false), logY(false), fit(false) {}

void Plot::render() {
    if (x.empty() || y.empty()) {
        return;
    }

    Rect rect = getRectPixels();
    ImGui::SetCursorPos(ImVec2(rect.x, rect.y));

    reset();

    if (ImPlot::BeginPlot(label.c_str(), ImVec2(rect.width, rect.height))) {
        if (logX) {
            ImPlot::SetupAxisScale(ImAxis_X1, ImPlotScale_Log10);
        }

        if (logY) {
            ImPlot::SetupAxisScale(ImAxis_Y1, ImPlotScale_Log10);
        }

        ImPlot::PlotLine(("##" + label).c_str(), x.data(), y.data(), x.size());
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
    fit = true;
}

void Plot::setLogY(bool logY) noexcept {
    this->logY = logY;
    fit = true;
}

void Plot::reset() {
    if (fit) {
        ImPlot::SetNextAxesToFit();
        fit = false;
    }
}