#ifndef FLOATING_EYE_ENUMSENSORWIDGET_H
#define FLOATING_EYE_ENUMSENSORWIDGET_H

#include "Sensor.h"
#include "IWidget.h"

#include "IrrIMGUI/IrrIMGUI.h"

#include <memory>
#include <string>
#include <vector>

class EnumSensorWidget :
    public IWidget
{
public:
    EnumSensorWidget(std::shared_ptr<Sensor<int>> sensor);

    virtual ~EnumSensorWidget() = default;

    virtual void draw() override;

private:
    static const int invalidState = -1;
    void drawLabels(int activeState) const;

    std::shared_ptr<Sensor<int>> sensor_;
    std::vector<std::string> labels_;
};

#endif // FLOATING_EYE_ENUMSENSORWIDGET_H
