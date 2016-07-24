#ifndef FLOATING_EYE_SENSOR_H
#define FLOATING_EYE_SENSOR_H

#include <ctime>
#include <mutex>
#include <stdexcept>
#include <string>

template<typename T>
class Sensor
{
 public:
    Sensor(const std::string& name, const std::string& unit) :
        name_{name},
        unit_{unit}
    {
    }

    Sensor(const Sensor& sensor) :
        name_{sensor.name_},
        unit_{sensor.unit_}
    {
    }

    virtual ~Sensor() = default;

    void updateValue(const T& value)
    {
        std::lock_guard<std::mutex> lock(valueMutex_);
        value_ = value;
        time(&lastUpdate_);
        valid_ = true;
    }

    T getValue() const
    {
        std::lock_guard<std::mutex> lock(valueMutex_);

        if (!valid_) {
            throw std::runtime_error("invalid value");
        }

        return value_;
    }

    /// returns seconds since last update
    double getTimeSinceLastUpdate() const
    {
        std::lock_guard<std::mutex> lock(valueMutex_);
        if (!valid_) {
            return 0;
        }

        time_t now = time(nullptr);
        return difftime(now, lastUpdate_);
    }

    std::string getName() const
    {
        return name_;
    }

    std::string getUnit() const
    {
        return unit_;
    }

    bool isValid() const
    {
        std::lock_guard<std::mutex> lock(valueMutex_);
        return valid_;
    }

 private:
    mutable std::mutex valueMutex_;
    bool valid_ = false;
    time_t lastUpdate_;
    const std::string name_;
    const std::string unit_; // TODO: use some cpp unit libs?
    T value_;
};

#endif // FLOATING_EYE_SENSOR_H
