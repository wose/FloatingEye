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
    ISS() :
        Vessel("ISS"),
        getUpdates_{true},
        lsc_{std::make_unique<LSClient>("https://push.lightstreamer.com", "", "", "ISSLIVE")},
        c1AVoltage_{std::make_shared<Sensor<double>>("Voltage", "V")},
        c2AVoltage_{std::make_shared<Sensor<double>>("Voltage", "V")},
        c3AVoltage_{std::make_shared<Sensor<double>>("Voltage", "V")},
        c4AVoltage_{std::make_shared<Sensor<double>>("Voltage", "V")}
    {
        lsc_->addSubscription(new LSSubscription("SPARTAN",
                                                 {"TIME_000001", "S4000001", "S4000002", "S4000007"},
                                                 {"TimeStamp","Value","Status.Class","Status.Indicator","Status.Color","CalibratedData"}));

        lsc_->start();
        updateThread_ = std::thread(&ISS::updateThread, this);
    }

    virtual ~ISS()
    {
        getUpdates_ = false;
        updateThread_.join();
    }

    void updateThread()
    {
        while (lsc_->getStatus() != LS_STATUS_CONNECTED && getUpdates_) {
            sleep(1);
        }

        if (getUpdates_) {
            lsc_->subscribeAll();
        }

        while (getUpdates_) {
            std::vector<LSSubscription*>* subscriptions = lsc_->getSubscriptions();

            for (size_t subscription = 0; subscription < subscriptions->size(); ++subscription) {
                auto tableRef = subscriptions->at(subscription)->table_ref;
                for (size_t index = 0; index <  subscriptions->at(subscription)->getObjectIds().size(); ++index) {
                    auto values = tableRef->getItemData(index);
                    if (subscriptions->at(subscription)->getObjectIds()[index] == "S4000001") {
                        if(values->size() > 1) {
                            c1AVoltage_->updateValue(stod(values->at(1).c_str()));
                        }
                    }
                }
            }
            sleep(1);
        }
    }

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
