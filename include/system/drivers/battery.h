#if !defined(__CODE_GRAV_SYSTEM_DRIVERS_BATTERY_H__)
#define __CODE_GRAV_SYSTEM_DRIVERS_BATTERY_H__

#include <system/utils/isensor.h>
#include <system/utils/types.h>

#define DEFAULT_BATTERY_PIN 10

class BatterySensor : public IBatterySensor
{
    uint8_t pin;
    bool firstReadDone;

    uint16_t readMilliVolts ();
    float readPercentage ();
public:
    BatterySensor (uint8_t pin = DEFAULT_BATTERY_PIN);

    bool init();
    bool update();
    void dumpInfo();
    SensorState_e checkState ();
};

#endif // __CODE_GRAV_SYSTEM_DRIVERS_BATTERY_H__
