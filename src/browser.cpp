#include "browser.hpp"

Browser::Browser(const Rect& rect, const std::string& label)
    : Widget(rect, label), currFile(-1), currData(-1) {}

void Browser::render() {
    Rect rect = getRectPixels();

    if (ImGui::TreeNode(label.c_str())) {
        for (int i = 0; i < files.size(); ++i) {
            if (ImGui::TreeNode(files[i].getName().c_str())) {

                for (int j = 0; j < files[i].getData().size(); ++j) {
                    if (ImGui::Selectable(files[i].getData()[j].name.c_str(), (currFile == i) && (currData == j))) {
                        currFile = i;
                        currData = j;
                    }
                }

                ImGui::TreePop();
            }
        }

        ImGui::TreePop();
    }
}

void Browser::add(File& file) {
    Parser::parse(file);
    files.push_back(file);
}

void Browser::setCallback(std::function<void()> callback) {
    this->callback = callback;
}

Data& Browser::getData() {
    return files[currFile].getData()[currData];
}