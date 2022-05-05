#if !defined(__CODE_GRAV_SRC_SYSTEM_KERNEL_INTERNAL_H__)
#define __CODE_GRAV_SRC_SYSTEM_KERNEL_INTERNAL_H__

#include "system/kernel/core.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

namespace OsKernel {

extern bool OsInitBuses();
extern bool OsInitLogger();
extern bool OsInitSensors();
extern bool OsInitTasks();
extern bool OsSetBootMode();
extern bool OsInitBLE();

extern SystemConfig_t   configuration;
extern SystemBootMode_t bootMode;

extern TaskHandle_t updateSensorsTask_h;
extern TaskHandle_t systemMainTask_h;

extern ISensor* rotationSensor;

} // namespace OsKernel

#endif // __CODE_GRAV_SRC_SYSTEM_KERNEL_INTERNAL_H__
