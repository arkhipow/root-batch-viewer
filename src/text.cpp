#include "text.hpp"

Text::Text(const Rect& rect, const std::string& label)
    : Widget(rect, label) {}

void Text::render() {
    auto rect = getRectPixels();
    ImGui::SetCursorPos(ImVec2(rect.x, rect.y));

    ImGui::BeginChild(label.c_str(), ImVec2(rect.width, rect.height));

    ImVec2 contentSize = ImGui::GetContentRegionAvail();
    ImVec2 textSize = ImGui::CalcTextSize(text.c_str());

    float textX = (contentSize.x - textSize.x) * 0.5f;
    float textY = (contentSize.y - textSize.y) * 0.5f;

    ImGui::SetCursorPos(ImVec2(textX, textY));

    ImGui::Text(text.c_str());

    ImGui::EndChild();
}

std::string Text::getText() const {
    return text;
}

void Text::setText(const std::string& text) {
    this->text = text;
}

Input::Input(const Rect& rect, const std::string& label)
    : Widget(rect, label), val(0) {}

void Input::render() {
    auto rect = getRectPixels();
    ImGui::SetCursorPos(ImVec2(rect.x, rect.y));
    ImGui::SetNextItemWidth(rect.width);

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, (rect.height - ImGui::GetFontSize()) / 2.0f));

    if (ImGui::InputInt(("##" + label).c_str(), &val, 0)) {
        if (callback) {
            callback();
        }
    }

    ImGui::PopStyleVar();
}

int Input::getVal() const noexcept {
    return val;
}

void Input::setVal(int val) noexcept {
    this->val = val;
}

void Input::setCallback(std::function<void()> callback) {
    this->callback = callback;
}