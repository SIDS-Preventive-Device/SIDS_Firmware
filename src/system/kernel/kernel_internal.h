#if !defined(__CODE_GRAV_SRC_SYSTEM_KERNEL_INTERNAL_H__)
#define __CODE_GRAV_SRC_SYSTEM_KERNEL_INTERNAL_H__

#include "system/kernel/core.h"
#include "system/kernel/calls.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define KERNEL_SERVICE_NAME(__service) __kernel_service ## __service
#define KERNEL_SERVICE(__service) void KERNEL_SERVICE_NAME(__service) (void *pContext)

#define KERNEL_CALLBACK_NAME(__cb) __kernel_callback ## __cb
#define KERNEL_CALLBACK(__cb) void KERNEL_CALLBACK_NAME(__cb) (void *pContext)

namespace OsKernel {

extern bool OsInitBuses();
extern bool OsInitLogger();
extern bool OsInitSensors();
extern bool OsInitTasks();
extern bool OsSetBootMode();
extern bool OsInitBLE();
extern bool NvStorageInit();

extern SystemConfig_t   configuration;
extern SystemBootMode_t bootMode;

extern TaskHandle_t updateSensorsTask_h;
extern TaskHandle_t systemMainTask_h;
extern esp_timer_handle_t rfshSensorState_h;

extern ISensor* rotationSensor;

} // namespace OsKernel

//
// EXTERNAL CALL SERVICES
//
extern KERNEL_SERVICE(OS_SERVICE_UPDATE_ORIENTATION);
extern KERNEL_SERVICE(OS_SERVICE_REBOOT);

//
// CALLBACKS
//
extern KERNEL_CALLBACK(UPDATE_SERVICES_CB);

#endif // __CODE_GRAV_SRC_SYSTEM_KERNEL_INTERNAL_H__
