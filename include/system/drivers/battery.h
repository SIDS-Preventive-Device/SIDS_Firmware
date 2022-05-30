#if !defined(__CODE_GRAV_SYSTEM_DRIVERS_BATTERY_H__)
#define __CODE_GRAV_SYSTEM_DRIVERS_BATTERY_H__

#include <system/utils/isensor.h>
#include <system/utils/types.h>

#define DEFAULT_BATTERY_PIN 34
#define BAT_SAMPLES_NUM 10

class BatterySensor : public IBatterySensor
{
    uint8_t pin;
    bool firstReadDone;

    int16_t adcMeasures[BAT_SAMPLES_NUM];

    uint16_t readMilliVolts ();
    float readPercentage ();
public:
    BatterySensor (uint8_t pin = DEFAULT_BATTERY_PIN);

    bool init();
    bool update();
    void dumpInfo();
    SensorState_e checkState ();

    float getRawMeasure() { return this->readMilliVolts(); };
};

#endif // __CODE_GRAV_SYSTEM_DRIVERS_BATTERY_H__
