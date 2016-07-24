#ifndef FLOATING_EYE_SENSORWIDGET_H
#define FLOATING_EYE_SENSORWIDGET_H

#include "IWidget.h"

#include "IrrIMGUI/IrrIMGUI.h"

#include <memory>
#include <vector>

class SensorWidget:
    public IWidget
{
public:
    SensorWidget(const std::string& name) :
        name_{name}
    {}

    virtual ~SensorWidget() = default;

    virtual void draw() override
    {
        ImGui::Begin(name_.c_str(), NULL,
                     ImGuiWindowFlags_NoCollapse
                     | ImGuiWindowFlags_NoResize
                     | ImGuiWindowFlags_NoMove
                     | ImGuiWindowFlags_NoBringToFrontOnFocus
                     | ImGuiWindowFlags_NoScrollbar);

        for (auto sensor : sensors_) {
            sensor->draw();
        }

        ImGui::End();
    }

    void addSensor(std::shared_ptr<IWidget> sensor)
    {
        sensors_.emplace_back(sensor);
    }

private:
    std::string name_;
    std::vector<std::shared_ptr<IWidget>> sensors_;
};

#endif // FLOATING_EYE_SENSORWIDGET_H
