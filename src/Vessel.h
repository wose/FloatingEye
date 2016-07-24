#ifndef FLOATING_EYE_VESSEL_H
#define FLOATING_EYE_VESSEL_H

#include "State.h"

#include <string>

class Vessel
{
public:
    Vessel(const std::string& name);
    virtual ~Vessel() = default;

    void updateState(const std::string& line1, const std::string& line2);
    State getState() const;
private:
    std::string name_;
    State state_;
};

#endif // FLOATING_EYE_VESSEL_H
