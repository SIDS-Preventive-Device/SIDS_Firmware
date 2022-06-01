#if !defined(__CODE_GRAV_SYSTEM_KERNEL_INIT_H__)
#define __CODE_GRAV_SYSTEM_KERNEL_INIT_H__

#include <stdint.h>
#include "bootmode.h"
#include "calls.h"
#include "system/modules/logger.h"
#include "system/utils/isensor.h"
#include <WString.h>

class TwoWire;
class HardwareSerial;

#define KERNEL_VERSION F("0.3.0")

typedef struct {
    uint32_t        baudrate;
    eLogLevel_t     logLevel;
    HardwareSerial* serialPort;

    TwoWire*        i2cBus;

    const __FlashStringHelper* bleDeviceName;

    IOrientationSensor *orientationSensor;
    IBatterySensor     *batterySensor;
    ITemperatureSensor *temperatureSensor;
    IRiskAlert         *riskAlert;
} SystemConfig_t;

namespace OsKernel
{

extern void OsInit(SystemConfig_t config);

extern SystemConfig_t* OsGetSysConfigPtr ();

extern bool OsDumpSysInfo();
extern SystemBootMode_t OsGetBootMode();

} // namespace OsKernel

#endif // __CODE_GRAV_SYSTEM_KERNEL_INIT_H__
