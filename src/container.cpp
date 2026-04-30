#include "container.hpp"

Container::Container(const Rect& rect, const std::string& label)
    : Widget(rect, label) {}

void Container::render() {
    auto rect = getRectPixels();
    ImGui::SetCursorPos(ImVec2(rect.x, rect.y));

    ImGui::BeginChild(label.c_str(), ImVec2(rect.width, rect.height));

    for (auto& widget : widgets) {
        if (widget->getVisible()) {
            widget->render();
        }
    }

    ImGui::EndChild();
}