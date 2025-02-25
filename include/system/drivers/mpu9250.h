#if !defined(__CODE_GRAV_SYSTEM_DRIVERS_MPU_9250_H__)
#define __CODE_GRAV_SYSTEM_DRIVERS_MPU_9250_H__

#include <system/utils/isensor.h>
#include <system/utils/types.h>

//
// Third-Party Library...
//
#include <MPU9250.h>

class SensorMPU9250 : public IOrientationSensor
{
private:
    bool firstReadDone;

public:
    SensorMPU9250 ();

    bool init();
    bool update();
    void dumpInfo();
    SensorState_e checkState ();
    MPU9250 device;
};

#endif // __CODE_GRAV_SYSTEM_DRIVERS_MPU_9250_H__
