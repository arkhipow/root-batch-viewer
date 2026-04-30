#pragma once

#include "widget.hpp"

#include <functional>

class Text : public Widget {
public:
    Text(const Rect& rect, const std::string& label);

    void render() override;

    std::string getText() const;
    void setText(const std::string& text);

protected:
    std::string text;
};

class Input : public Widget {
public:
    Input(const Rect& rect, const std::string& label);

    void render() override;

    int getVal() const noexcept;
    void setVal(int val) noexcept;

    void setCallback(std::function<void()> callback);

private:
    int val;

    std::function<void()> callback;
};