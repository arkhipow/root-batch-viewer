#include "button.hpp"

Button::Button(const Rect& rect, const std::string& label)
    : Widget(rect, label) {}

void Button::render() {
    auto rect = getRectPixels();
    ImGui::SetCursorPos(ImVec2(rect.x, rect.y));

    if (ImGui::Button(label.c_str(), ImVec2(rect.width, rect.height))) {
        if (callback) {
            callback();
        }
    }
}

void Button::setCallback(std::function<void()> callback) {
    this->callback = callback;
}

ToggleButton::ToggleButton(const Rect& rect, const std::string& label)
    : Button(rect, label), toggle(false) {}

void ToggleButton::render() {
    auto rect = getRectPixels();
    ImGui::SetCursorPos(ImVec2(rect.x, rect.y));

    if (toggle) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]);
    }

    else {
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_Button]);
    }

    if (ImGui::Button(label.c_str(), ImVec2(rect.width, rect.height))) {
        toggle = !toggle;

        if (callback) {
            callback();
        }
    }

    ImGui::PopStyleColor();
}

bool ToggleButton::getToggle() const noexcept {
    return toggle;
}

void ToggleButton::setToggle(bool toggle) noexcept {
    this->toggle = toggle;
}