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
    MPU9250 device;
    uint8_t id;

    bool firstReadDone;

public:
    SensorMPU9250 ();

    bool init();
    bool update();
    void dumpInfo();
};

#endif // __CODE_GRAV_SYSTEM_DRIVERS_MPU_9250_H__
