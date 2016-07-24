#include "Vessel.h"

Vessel::Vessel(const std::string& name) :
    name_{name}
{
}

void Vessel::updateState(const std::string& line1,const std::string& line2)
{
    state_.update(line1, line2);
}

State Vessel::getState() const
{
    return state_;
}
