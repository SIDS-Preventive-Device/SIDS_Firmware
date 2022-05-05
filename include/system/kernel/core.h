#if !defined(__CODE_GRAV_SYSTEM_KERNEL_INIT_H__)
#define __CODE_GRAV_SYSTEM_KERNEL_INIT_H__

#include <stdint.h>
#include "bootmode.h"
#include "calls.h"
#include "system/modules/logger.h"
#include "system/utils/isensor.h"

class TwoWire;
class HardwareSerial;

#define KERNEL_VERSION F("0.2.0")

typedef struct {
    uint32_t        baudrate;
    eLogLevel_t     logLevel;
    HardwareSerial* serialPort;

    TwoWire*        i2cBus;

    IOrientationSensor *orientationSensor;
} SystemConfig_t;

namespace OsKernel
{

extern void OsInit(SystemConfig_t config);
extern void OsCall(KernelService_e);

extern SystemConfig_t* OsGetSysConfigPtr ();

extern bool OsDumpSysInfo();
extern SystemBootMode_t OsGetBootMode();

} // namespace OsKernel

#endif // __CODE_GRAV_SYSTEM_KERNEL_INIT_H__
