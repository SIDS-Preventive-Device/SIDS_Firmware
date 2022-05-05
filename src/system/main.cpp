#include "system/kernel/core.h"
#include "system/utils/types.h"
#include "system/modules/orientation.h"
#include "system/modules/logger.h"

#include <BluetoothSerial.h>

/**
 * @brief Construct a new kernel boot thread this is the entry point for
 *        boot mode normal.
 * 
 */
KERNEL_BOOT_THREAD_FUNC(BOOT_NORMAL) {
    logger << LOG_MASTER << F("Normal mode enabled!") << EndLine;

    SystemConfig_t *pConfig = OsKernel::OsGetSysConfigPtr();

    while (true) {
        pConfig->orientationSensor->update();
        vTaskDelay(pdMS_TO_TICKS(1000UL));
    }
}

/**
 * @brief Construct a new kernel boot thread this is the entry point for
 *        boot mode calibration.
 * 
 */
KERNEL_BOOT_THREAD_FUNC(BOOT_CALIBRATION) {
    logger << LOG_MASTER << F("Calibration mode enabled!") << EndLine;

    while (true) {

    }
}
