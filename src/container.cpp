#include "container.hpp"

Container::Container(const Rect& rect, const std::string& label)
    : Widget(rect, label)
{
    ImGuiStyle& style = ImGui::GetStyle();
    color = style.Colors[ImGuiCol_ChildBg];
}

void Container::render() {
    auto rect = getRectPixels();
    ImGui::SetCursorPos(ImVec2(rect.x, rect.y));

    ImGui::PushStyleColor(ImGuiCol_ChildBg, color);

    ImGui::BeginChild(label.c_str(), ImVec2(rect.width, rect.height));

    for (auto& widget : widgets) {
        if (widget->getVisible()) {
            widget->render();
        }
    }

    ImGui::EndChild();

    ImGui::PopStyleColor();
}

void Container::setColor(float r, float g, float b, float a) {
    color = { r, g, b, a };
}