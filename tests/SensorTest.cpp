#include "Sensor.h"

#include "gmock/gmock.h"

#include <string>

using namespace ::testing;

class ASensor : public Test
{
public:
    const std::string ASensorName{"Sensor"};
    const std::string AnSensorUnit{"mm"};
    const int ASensorValue{42};
    Sensor<int> AnIntSensor{"Int Sensor", "pixel"};
};

TEST_F(ASensor, canBeConstructedWithNameAndUnit)
{
    Sensor<int> s{ASensorName, AnSensorUnit};
    ASSERT_THAT(s.getName(), StrEq(ASensorName));
    ASSERT_THAT(s.getUnit(), StrEq(AnSensorUnit));
}

TEST_F(ASensor, throwsWhenValueWasntOnceUpdatedOnGet)
{
    ASSERT_THROW(AnIntSensor.getValue(), std::runtime_error);
}

TEST_F(ASensor, canReturnItsValue)
{
    AnIntSensor.updateValue(ASensorValue);
    ASSERT_THAT(AnIntSensor.getValue(), Eq(ASensorValue));
}

TEST_F(ASensor, isInvalidWhenNotUpdated)
{
    ASSERT_THAT(AnIntSensor.isValid(), Eq(false));
}

TEST_F(ASensor, isValidAfterUpdate)
{
    AnIntSensor.updateValue(ASensorValue);
    ASSERT_THAT(AnIntSensor.isValid(), Eq(true));
}
