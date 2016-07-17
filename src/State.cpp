#include "State.h"

State::State(const std::string& tle /* = "" */)
{
    update(tle);
}

void State::update(const std::string& tle)
{

}

bool State::isValid() const
{
    return false;
}
