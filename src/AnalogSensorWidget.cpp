#include "AnalogSensorWidget.h"

AnalogSensorWidget::AnalogSensorWidget(std::shared_ptr<Sensor<double>> sensor,
                                       SWStyle style):
    style_{style},
    sensor_{sensor}
{
}

void AnalogSensorWidget::draw()
{
    sensor_->updateValue(185.42);
    if (sensor_->isValid()) {
        double value = sensor_->getValue();
        //            ImGui::Text("%s", sensor_->getName().c_str());
        ImGui::Text("%5.2lf %s", value, sensor_->getUnit().c_str());
        auto start = ImGui::GetCursorScreenPos();
        auto drawList = ImGui::GetWindowDrawList();
        auto width = ImGui::GetContentRegionAvailWidth();

        if(style_ == SWStyle::Linear) {
            ImGui::Dummy(ImVec2(ImGui::GetContentRegionAvailWidth(), 10.0));
            drawList->AddRectFilled(ImVec2(start.x, start.y + 2),
                                    ImVec2(start.x + ImGui::GetContentRegionAvailWidth(), start.y + 10.0 - 2),
                                    ImColor(80, 80, 80, 255));
            const double lower = 137.0 - 100;
            const double upper = 173.0 - 100;
            double range = upper - lower;
            double scale = width / 100;

            // lower bound
            ImColor boundColor = value <= lower + 100 ? ImColor(200, 0, 0, 255) : ImColor(140, 140, 140, 255);
            //ImColor(140, 140, 140, 255);
            //ImColor(41, 127, 127, 255);
            drawList->AddRectFilled(ImVec2(start.x, start.y + 2),
                                    ImVec2(start.x + lower * scale, start.y + 10.0 - 2),
                                    boundColor);
            // upper bound
            boundColor = value >= upper + 100 ? ImColor(200, 0, 0, 255) : ImColor(140, 140, 140, 255);
            drawList->AddRectFilled(ImVec2(start.x + upper * scale, start.y + 2),
                                    ImVec2(start.x + width, start.y + 10.0 - 2),
                                    boundColor);
            drawList->AddRectFilled(ImVec2(start.x + (value - 100) * scale - 1, start.y),
                                    ImVec2(start.x + (value - 100) * scale + 1, start.y + 10),
                                    ImColor(64, 196, 196, 255));
        } else {
            ImGui::Dummy(ImVec2(width, 40.0));
            ImVec2 center{start.x + width / 2, start.y + 20};

            drawList->AddCircle(center, 15.0,
                                ImColor(80, 80, 80, 255), 16, 3.0f);
            drawList->AddLine(ImVec2(center.x + cos(value * M_PI / 180.0) * 10,
                                     center.y + sin(value * M_PI / 180.0) * -10),
                              ImVec2(center.x + cos(value * M_PI / 180.0) * 20,
                                     center.y + sin(value * M_PI / 180.0) * -20),
                              ImColor(64, 196, 196, 255), 3);
        }
    } else {
        ImGui::Text("%s", sensor_->getName().c_str());
        ImGui::Text("N/A");
    }
}
