#if !defined(__CODE_GRAV_SYSTEM_DRIVERS_BATTERY_H__)
#define __CODE_GRAV_SYSTEM_DRIVERS_BATTERY_H__

#include <system/utils/isensor.h>
#include <system/utils/types.h>

class BatterySensor : public IBatterySensor
{
public:
    BatterySensor ();

    bool init();
    bool update();
    void dumpInfo();
    SensorState_e checkState ();
};

#endif // __CODE_GRAV_SYSTEM_DRIVERS_BATTERY_H__
