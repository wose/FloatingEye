#ifndef FLOATING_EYE_ANALOGSENSORWIDGET_H
#define FLOATING_EYE_ANALOGSENSORWIDGET_H

#include "Sensor.h"
#include "IWidget.h"

#include "IrrIMGUI/IrrIMGUI.h"

#include <memory>

enum class SWStyle
{
    Linear,
    LinearWithBounds,
    Rotary,
    RotaryHalf
};

class AnalogSensorWidget :
    public IWidget
{
public:
    AnalogSensorWidget(std::shared_ptr<Sensor<double>> sensor, SWStyle style = SWStyle::Linear);

    virtual ~AnalogSensorWidget() = default;

    virtual void draw() override;

private:
    SWStyle style_;
    std::shared_ptr<Sensor<double>> sensor_;
};

#endif // FLOATING_EYE_ANALOGSENSORWIDGET_H
