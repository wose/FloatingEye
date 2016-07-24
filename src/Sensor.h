#ifndef FLOATING_EYE_SENSOR_H
#define FLOATING_EYE_SENSOR_H

#include <exception>
#include <string>
#include <time>

template<typename T>
class Sensor
{
 public:
    Sensor(const std::string& name, const std::string& unit) :
        name_{name},
        unit_{unit}
    {
    }

    virtual ~Sensor() = default;

    void updateValue(const T& value)
    {
        value_ = value;
        time(&lastUpdate_);
    }

    T getValue() const
    {
        if (!valid_) {
            throw std::runtime_error("invalid value");
        }

        return value_;
    }

    /// returns seconds since last update
    double getTimeSinceLastUpdate() const
    {
        if (!valid_) {
            return 0;
        }

        time_t now = time(nullptr);
        return difftime(now, lastUpdate_);
    }

    string getName() const
    {
        return name_;
    }

    string getUnit() const
    {
        return unit_;
    }

    bool isValid() const
    {
        return valid_;
    }
 private:
    bool valid_;
    time_t lastUpdate_;
    std::string name_;
    std::string unit_; // TODO: use some cpp unit libs?
    T value_;
};

#endif // FLOATING_EYE_SENSOR_H
