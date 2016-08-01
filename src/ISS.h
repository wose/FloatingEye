#ifndef FLOATING_EYE_ISS_H
#define FLOATING_EYE_ISS_H

#include "Sensor.h"
#include "Vessel.h"

#include "lsclient.hpp"

#include <atomic>
#include <memory>
#include <thread>


class ISS :
    public Vessel
{
public:
    ISS();

    virtual ~ISS();

    void updateThread();

public:
    std::atomic<bool> getUpdates_;
    std::unique_ptr<LSClient> lsc_;
    std::thread updateThread_;

    // SPARTAN
    std::shared_ptr<Sensor<double>> c1AVoltage_;
    std::shared_ptr<Sensor<double>> c2AVoltage_;
    std::shared_ptr<Sensor<double>> c3AVoltage_;
    std::shared_ptr<Sensor<double>> c4AVoltage_;

    std::shared_ptr<Sensor<double>> c1BVoltage_;
    std::shared_ptr<Sensor<double>> c2BVoltage_;
    std::shared_ptr<Sensor<double>> c3BVoltage_;
    std::shared_ptr<Sensor<double>> c4BVoltage_;

    std::shared_ptr<Sensor<double>> c1ACurrent_;
    std::shared_ptr<Sensor<double>> c2ACurrent_;
    std::shared_ptr<Sensor<double>> c3ACurrent_;
    std::shared_ptr<Sensor<double>> c4ACurrent_;

    std::shared_ptr<Sensor<double>> c1BCurrent_;
    std::shared_ptr<Sensor<double>> c2BCurrent_;
    std::shared_ptr<Sensor<double>> c3BCurrent_;
    std::shared_ptr<Sensor<double>> c4BCurrent_;

    std::shared_ptr<Sensor<double>> c1APosition_;
    std::shared_ptr<Sensor<double>> c2APosition_;
    std::shared_ptr<Sensor<double>> c3APosition_;
    std::shared_ptr<Sensor<double>> c4APosition_;

    std::shared_ptr<Sensor<double>> c1BPosition_;
    std::shared_ptr<Sensor<double>> c2BPosition_;
    std::shared_ptr<Sensor<double>> c3BPosition_;
    std::shared_ptr<Sensor<double>> c4BPosition_;

    std::shared_ptr<Sensor<double>> portJointPosition_;
    std::shared_ptr<Sensor<double>> starboardJointPosition_;
};

#endif // FLOATING_EYE_ISS_H
