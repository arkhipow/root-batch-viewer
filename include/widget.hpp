#pragma once

#include "rect.hpp"

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <implot.h>

#include <string>

class Widget {
public:
    Widget(const Rect& rect, const std::string& label);
    virtual ~Widget() = default;

    virtual void render() = 0;

    int getTab() const noexcept;
    void setTab(int tab) noexcept;

    bool getVisible() const noexcept;
    void setVisible(bool visible) noexcept;

protected:
    Rect getRectPixels() const;

    Rect rect;
    std::string label;

    int tab;
    bool visible;
};