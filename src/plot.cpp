#include "plot.hpp"

#include <TCanvas.h>
#include <TGraph.h>
#include <TH1F.h>
#include <TROOT.h>

#include <algorithm>

Graph::Graph(const Rect& rect, const std::string& label)
    : Widget(rect, label), measure(false), axisX("X"), axisY("Y") {}

void Graph::reset() {
    ImPlot::SetNextAxesToFit();
}

bool Graph::getMeasure() const noexcept {
    return measure;
}

void Graph::setMeasure(bool measure) noexcept {
    this->measure = measure;
}

std::string Graph::getAxisX() const {
    return axisX;
}

void Graph::setAxisX(const std::string& x) {
    this->axisX = x;
}

std::string Graph::getAxisY() const {
    return axisY;
}

void Graph::setAxisY(const std::string& y) {
    this->axisY = y;
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

        ImPlot::SetupAxis(ImAxis_X1, axisX.c_str());
        ImPlot::SetupAxis(ImAxis_Y1, axisY.c_str());

        ImPlot::PlotLine(("##" + label).c_str(), x.data(), y.data(), x.size(), spec);

        if (measure) {
            if (ImPlot::IsPlotHovered()) {
                auto [posX, posY] = ImPlot::GetPlotMousePos();
                int i = findClosestPoint(posX, posY);

                if (i >= 0) {
                    ImGui::BeginTooltip();
                    ImGui::Text("X: %g", x[i]);
                    ImGui::Text("Y: %g", y[i]);
                    ImGui::EndTooltip();

                    spec.MarkerSize = 8;
                    spec.MarkerFillColor = ImVec4(1, 0, 0, 0.5);
                    spec.LineColor = ImVec4(1, 0, 0, 0.5);
                    ImPlot::PlotLine(("##" + label + " Current").c_str(), &x[i], &y[i], 1, spec);

                    spec.MarkerSize = 4;

                    float lineV[4] = { x[i], x[i], y[i], (float)ImPlot::GetPlotLimits().Y.Min };
                    ImPlot::PlotLine(("##" + label + " V").c_str(), lineV, lineV + 2, 2, spec);

                    float lineH[4] = { x[i], (float)ImPlot::GetPlotLimits().X.Min, y[i], y[i] };
                    ImPlot::PlotLine(("##" + label + " H").c_str(), lineH, lineH + 2, 2, spec);
                }
            }
        }

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

int Plot::findClosestPoint(float mPosX, float mPosY) {
    int i = -1;
    float dist = FLT_MAX;

    for (float p : x) {
        float d = std::abs(mPosX - p);

        if (d < dist) {
            dist = d;
            i++;
        }

        else {
            break;
        }
    }

    if (mPosY < y[i] - 2 || mPosY > y[i] + 2) {
        i = -1;
    }

    return i;
}

Histogram::Histogram(const Rect& rect, const std::string& label)
    : Graph(rect, label), bins(-2) {}

void Histogram::render() {
    Rect rect = getRectPixels();
    ImGui::SetCursorPos(ImVec2(rect.x, rect.y));

    if (ImPlot::BeginPlot(label.c_str(), ImVec2(rect.width, rect.height))) {
        ImPlotSpec spec;
        spec.FillAlpha = 0.5;

        float binWidth = 1.0f;
        if (y.size() > 1) {
            binWidth = y[1] - y[0];
        }

        ImPlot::SetupAxis(ImAxis_X1, axisX.c_str());
        ImPlot::SetupAxis(ImAxis_Y1, axisY.c_str());

        ImPlot::PlotBars(("##" + label).c_str(), y.data(), x.data(), y.size(), binWidth, spec);

        if (measure && ImPlot::IsPlotHovered()) {
            auto [posX, posY] = ImPlot::GetPlotMousePos();
            int i = findClosestBin(posX, posY);

            if (i >= 0) {
                ImGui::BeginTooltip();
                ImGui::Text("DX: %g", binWidth);
                ImGui::Text("CX: %g", y[i]);
                ImGui::Text("Y: %g", x[i]);
                ImGui::EndTooltip();

                spec.FillColor = ImVec4(1, 0, 0, 1);
                ImPlot::PlotBars(("##" + label + " Current").c_str(), &y[i], &x[i], 1, binWidth, spec);
            }
        }

        ImPlot::EndPlot();
    }
}

std::vector<float> Histogram::getX() const {
    return x;
}

std::vector<float> Histogram::getY() const {
    return y;
}

void Histogram::setData(const std::vector<float>& x, const std::vector<float>& y) {
    this->x = x;
    this->y = y;
}

void Histogram::setBins(int bins) noexcept {
    this->bins = bins;
}

int Histogram::findClosestBin(float mPosX, float mPosY) {
    int i = -1;

    float binWidth = 1.0f;
    if (y.size() > 1) {
        binWidth = y[1] - y[0];
    }

    for (int j = 0; j < y.size(); ++j) {
        if (mPosX >= y[j] - binWidth / 2 && mPosX <= y[j] + binWidth / 2) {
            if (mPosY >= 0 && mPosY <= x[j]) {
                i = j;
            }
        }
    }

    return i;
}

void Plot::save(const std::string& path) {
    gROOT->SetBatch(kTRUE);

    TCanvas* canvas = new TCanvas("canvas_plot", "CERN ROOT Canvas", 1200, 900);

    TGraph* graph = new TGraph(static_cast<int>(x.size()), x.data(), y.data());

    std::string titleStr = label;
    graph->SetTitle(titleStr.c_str());

    graph->GetXaxis()->SetTitle(axisX.c_str());
    graph->GetYaxis()->SetTitle(axisY.c_str());

    graph->SetLineColor(kBlue + 2);
    graph->SetLineWidth(2);

    graph->Draw("AL");

    canvas->SaveAs(path.c_str());

    delete graph;
    delete canvas;

    gROOT->SetBatch(kFALSE);
}

void Histogram::save(const std::string& path) {
    gROOT->SetBatch(kTRUE);

    TCanvas* canvas = new TCanvas("canvas_hist", "CERN ROOT Canvas", 1200, 900);

    canvas->SetLogy(0);

    float binWidth = 1.0f;
    if (y.size() > 1) {
        binWidth = y[1] - y[0];
    }

    float lowEdge = y.front() - (binWidth / 2.0f);
    float highEdge = y.back() + (binWidth / 2.0f);
    int nBins = static_cast<int>(y.size());

    std::string titleStr = label;
    TH1F* hist = new TH1F("h_root", titleStr.c_str(), nBins, lowEdge, highEdge);

    hist->GetXaxis()->SetTitle(axisX.c_str());
    hist->GetYaxis()->SetTitle(axisY.c_str());

    for (int i = 0; i < nBins; ++i) {
        hist->SetBinContent(i + 1, x[i]);
    }

    hist->SetFillColor(kBlue - 7);
    hist->SetLineColor(kBlack);
    hist->SetLineWidth(1);

    hist->Draw("HIST");

    canvas->SaveAs(path.c_str());

    delete hist;
    delete canvas;

    gROOT->SetBatch(kFALSE);
}