#pragma once

#include "widget.hpp"

#include <functional>

class Button : public Widget {
public:
    Button(const Rect& rect, const std::string& label);

    void render() override;

    void setCallback(std::function<void()> callback);

protected:
    std::function<void()> callback;
};

class ToggleButton : public Button {
public:
    ToggleButton(const Rect& rect, const std::string& label);

    void render() override;

    bool getToggle() const noexcept;
    void setToggle(bool toggle) noexcept;

private:
    bool toggle;
};