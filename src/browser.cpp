#include "browser.hpp"

Browser::Browser(const Rect& rect, const std::string& label)
    : Widget(rect, label), currFile(-1), currData(-1) {}

void Browser::render() {
    Rect rect = getRectPixels();
    elementRect = elementRect.toPixels(rect.width, rect.height);

    ImGui::SetWindowFontScale(getScale() * 0.8);

    if (ImGui::TreeNode(label.c_str())) {
        for (int i = 0; i < files.size(); ++i) {
            if (ImGui::TreeNode(files[i].getName().c_str())) {
                File& file = files[i];

                for (int j = 0; j < file.getData().size(); ++j) {
                    Data& data = file.getData()[j];
                    std::string typeName = "[" + data.cname + "] ";

                    if (ImGui::Selectable((typeName + data.name).c_str(), (currFile == i) && (currData == j), 0, ImVec2(elementRect.width, elementRect.height))) {
                        currFile = i;
                        currData = j;
                    }
                }

                ImGui::TreePop();
            }
        }

        ImGui::TreePop();
    }

    ImGui::SetWindowFontScale(1);
}

void Browser::add(File& file) {
    Parser::parse(file);
    files.push_back(file);
}

void Browser::setCallback(std::function<void()> callback) {
    this->callback = callback;
}

void Browser::setElementRect(Rect rect) {
    this->elementRect = rect;
}

Data& Browser::getData() {
    return files[currFile].getData()[currData];
}