#pragma once

#include "widget.hpp"

#include <vector>

class Plot : public Widget {
public:
    Plot(const Rect& rect, const std::string& label);

    void render() override;

    void setData(const std::vector<float>& x, const std::vector<float>& y);

    bool getLogX() const noexcept;
    bool getLogY() const noexcept;

    void setLogX(bool logX) noexcept;
    void setLogY(bool logY) noexcept;

private:
    void reset();

    std::vector<float> x;
    std::vector<float> y;

    bool logX;
    bool logY;

    bool fit;
};