#pragma once

#include "widget.hpp"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <memory>
#include <vector>

class Application final {
public:
    Application(const Rect& rect, const std::string& title);
    ~Application();

    void run();

    template <typename T, typename... Args>
    std::shared_ptr<T> add(Args&&... args);

private:
    void init();
    void destroy();

    void menu();
    void render();

    template <typename T>
    using Items = std::vector<std::shared_ptr<T>>;

    enum class menuTabs {
        File, Process, Plots, View
    };

    Rect rect;
    std::string title;

    Items<Widget> widgets;
    menuTabs tab;

    GLFWwindow* handle;
};

template <typename T, typename... Args>
std::shared_ptr<T> Application::add(Args&&... args) {
    auto widget = std::make_shared<T>(std::forward<Args>(args)...);

    widgets.push_back(widget);

    return widget;
}