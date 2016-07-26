#include "EnumSensorWidget.h"

EnumSensorWidget::EnumSensorWidget(std::shared_ptr<Sensor<int>> sensor) :
    labels_{"OPEN", "CLOSED", "IN TRANSIT", "FAILED"},
    sensor_{sensor}
{
}

void EnumSensorWidget::draw()
{
    sensor_->updateValue(1);
    if (sensor_->isValid()) {
        drawLabels(sensor_->getValue());
    } else {
        drawLabels(invalidState);
    }
}

void EnumSensorWidget::drawLabels(int activeState) const
{
    ImVec2 start = ImGui::GetCursorPos();
    auto width = ImGui::GetContentRegionAvailWidth();
    auto height = ImGui::GetContentRegionAvail().y;
    auto drawList = ImGui::GetWindowDrawList();

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0, 12.0));

    auto leftTop = ImGui::GetCursorScreenPos();
    drawList->AddLine(leftTop, ImVec2(leftTop.x, leftTop.y + height),
                      ImColor(80, 80, 80, 255), 2.0f);
    drawList->AddLine(ImVec2(leftTop.x + width, leftTop.y),
                      ImVec2(leftTop.x + width, leftTop.y + height),
                      ImColor(80, 80, 80, 255), 2.0f);

    for (int label = 0; label < labels_.size(); ++label) {
        auto textSize = ImGui::CalcTextSize(labels_[label].c_str());
        auto lineStart = ImGui::GetCursorScreenPos();

        ImGui::SetCursorPosX(start.x + width / 2 - textSize.x / 2);

        // TODO: check if we need to separate the next value from the rest
        if (label == 2) {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0, 32.0));
        }

        if (label == activeState) {
            ImGui::TextColored(ImColor(64, 196, 196, 255), "%s", labels_[label].c_str());
            drawList->AddRect(ImVec2(lineStart.x + 8, lineStart.y - 2),
                              ImVec2(lineStart.x + width - 8, lineStart.y + textSize.y + 2),
                              ImColor(64, 196, 196, 255));
            drawList->AddLine(ImVec2(lineStart.x, lineStart.y + textSize.y / 2),
                              ImVec2(lineStart.x + 8, lineStart.y + textSize.y / 2),
                              ImColor(64, 196, 196, 255));
            drawList->AddLine(ImVec2(lineStart.x + width, lineStart.y + textSize.y / 2),
                              ImVec2(lineStart.x + width - 8, lineStart.y + textSize.y / 2),
                              ImColor(64, 196, 196, 255));
            drawList->AddLine(lineStart, ImVec2(lineStart.x, lineStart.y + textSize.y),
                              ImColor(64, 196, 196, 255), 2.0);
            drawList->AddLine(ImVec2(lineStart.x + width, lineStart.y),
                              ImVec2(lineStart.x + width, lineStart.y + textSize.y),
                              ImColor(64, 196, 196, 255), 2.0);
        } else {
            ImGui::TextColored(ImColor(41, 127, 127, 255), "%s", labels_[label].c_str());
        }
    }

    ImGui::PopStyleVar(2);
}
