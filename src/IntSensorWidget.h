#ifndef FLOATING_EYE_INTSENSORWIDGET_H
#define FLOATING_EYE_INTSENSORWIDGET_H

#include "Sensor.h"
#include "IWidget.h"

#include "IrrIMGUI/IrrIMGUI.h"

class IntSensorWidget :
    public IWidget
{
public:
    IntSensorWidget(std::shared_ptr<Sensor<int>> sensor) :
        sensor_{sensor}
    {}

    virtual ~IntSensorWidget() = default;

    virtual void draw() override
    {
        sensor_->updateValue(static_cast<int>(ImGui::GetIO().DeltaTime * 100));
        int value = sensor_->getValue();
        ImGui::Text("%s", sensor_->getName().c_str());
        ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() * 0.5f);
        ImGui::ProgressBar(100.0f / static_cast<float>(value),
                           ImVec2(ImGui::GetContentRegionAvailWidth() * 0.5, 0),
                           "");
        ImGui::SameLine();
        ImGui::Text("%d %s", value, sensor_->getUnit().c_str());
        ImGui::PopItemWidth();
    }

private:
    std::shared_ptr<Sensor<int>> sensor_;
};

#endif // FLOATING_EYE_INTSENSORWIDGET_H
