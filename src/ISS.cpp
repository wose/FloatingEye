#include "ISS.h"

ISS::ISS() :
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

ISS::~ISS()
{
    getUpdates_ = false;
    updateThread_.join();
}

void ISS::updateThread()
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
