#pragma once

#include "widget.hpp"

#include <memory>
#include <vector>

class Container : public Widget {
public:
    Container(const Rect& rect, const std::string& label);

    void render() override;

    template <typename T, typename... Args>
    std::shared_ptr<T> add(Args&&... args);

private:
    template <typename T>
    using Items = std::vector<std::shared_ptr<T>>;

    Items<Widget> widgets;
};

template <typename T, typename... Args>
std::shared_ptr<T> Container::add(Args&&... args) {
    auto widget = std::make_shared<T>(std::forward<Args>(args)...);
    widget->setTab(getTab());

    widgets.push_back(widget);

    return widget;
}