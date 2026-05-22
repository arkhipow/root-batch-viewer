#include "widget.hpp"

Widget::Widget(const Rect& rect, const std::string& label)
    : rect(rect), label(label), tab(0), visible(true) {}

int Widget::getTab() const noexcept {
    return tab;
}

void Widget::setTab(int tab) noexcept {
    this->tab = tab;
}

bool Widget::getVisible() const noexcept {
    return visible;
}

void Widget::setVisible(bool visible) noexcept {
    this->visible = visible;
}

std::string Widget::getLable() const {
    return label;
}

void Widget::setLable(const std::string& label) {
    this->label = label;
}

Rect Widget::getRectPixels() const {
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();

    float scaleX;
    float scaleY;
    glfwGetMonitorContentScale(monitor, &scaleX, &scaleY);

    ImVec2 size = ImGui::GetWindowSize();

    Rect rectPixels = rect.toPixels(
        size.x,
        size.y
    );

    if (rect.type == Rect::Type::Pixels) {
        rectPixels.width *= scaleX;
        rectPixels.height *= scaleY;
    }

    return rectPixels;
}

float Widget::getScale() const {
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();

    float scale;
    glfwGetMonitorContentScale(monitor, &scale, nullptr);

    return scale;
}