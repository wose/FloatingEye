#ifndef FLOATING_EYE_DOUBLESENSORWIDGET_H
#define FLOATING_EYE_DOUBLESENSORWIDGET_H

#include "Sensor.h"
#include "IWidget.h"

#include "IrrIMGUI/IrrIMGUI.h"

class DoubleSensorWidget :
    public IWidget
{
public:
    DoubleSensorWidget(std::shared_ptr<Sensor<double>> sensor) :
        sensor_{sensor}
    {}

    virtual ~DoubleSensorWidget() = default;

    virtual void draw() override
    {
        sensor_->updateValue(ImGui::GetIO().DeltaTime);
        double value = sensor_->getValue();
        ImGui::Text("%s", sensor_->getName().c_str());
        ImGui::ProgressBar(10.0f * value,
                           ImVec2(ImGui::GetContentRegionAvailWidth() * 0.3, 0),
                           "");
        ImGui::SameLine();
        ImGui::Text("%.2lf %s", value, sensor_->getUnit().c_str());
    }

private:
    std::shared_ptr<Sensor<double>> sensor_;
};

#endif // FLOATING_EYE_DOUBLESENSORWIDGET_H
