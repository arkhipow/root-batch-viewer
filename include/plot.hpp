#pragma once

#include "widget.hpp"

#include <vector>

class Graph : public Widget {
public:
    Graph(const Rect& rect, const std::string& label);

    static void reset();

    bool getMeasure() const noexcept;
    void setMeasure(bool measure) noexcept;

    std::string getAxisX() const;
    void setAxisX(const std::string& x);

    std::string getAxisY() const;
    void setAxisY(const std::string& y);

protected:
    bool measure;

    std::string axisX;
    std::string axisY;
};

class Plot : public Graph {
public:
    Plot(const Rect& rect, const std::string& label);

    void render() override;

    void setData(const std::vector<float>& x, const std::vector<float>& y);

    bool getLogX() const noexcept;
    bool getLogY() const noexcept;

    void setLogX(bool logX) noexcept;
    void setLogY(bool logY) noexcept;

    void save(const std::string& path);

private:
    int findClosestPoint(float mPosX, float mPosY);

    std::vector<float> x;
    std::vector<float> y;

    bool logX;
    bool logY;
};

class Histogram : public Graph {
public:
    Histogram(const Rect& rect, const std::string& label);

    void render() override;

    std::vector<float> getX() const;
    std::vector<float> getY() const;
    void setData(const std::vector<float>& x, const std::vector<float>& y);

    void setBins(int bins) noexcept;

    void save(const std::string& path);

private:
    int findClosestBin(float mPosX, float mPosY);

    std::vector<float> x;
    std::vector<float> y;
    int bins;
};