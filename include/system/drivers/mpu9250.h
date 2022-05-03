#if !defined(__CODE_GRAV_SYSTEM_DRIVERS_MPU_9250_H__)
#define __CODE_GRAV_SYSTEM_DRIVERS_MPU_9250_H__

#include <system/isensor.h>

//
// Third-Party Library...
//
#include <MPU9250.h>

class SensorMPU9250 : public ISensor
{
private:
    MPU9250 device;
    uint8_t id;

public:
    SensorMPU9250 ();
    bool init();
    bool update();
    bool haveVariable(Variable_e);
    void dumpInfo();
};

#endif // __CODE_GRAV_SYSTEM_DRIVERS_MPU_9250_H__
