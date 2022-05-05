#include "system/core.h"
#include "system/logger.h"
#include "system/types.h"
#include "system/errors.h"
#include "system/modules/orientation.h"

KERNEL_BOOT_THREAD_FUNC(BOOT_NORMAL) {
    if (!pKernel) {
        LogCriticalError ("Entrypoint is wrong, please execute kernel first");
    }

    logger << LOG_MASTER << F("Normal mode enabled!") << EndLine;

    while (true) {

    }
}

KERNEL_BOOT_THREAD_FUNC(BOOT_CALIBRATION) {
    if (!pKernel) {
        LogCriticalError ("Entrypoint is wrong, please execute kernel first");
    }

    logger << LOG_MASTER << F("Calibration mode enabled!") << EndLine;

    while (true) {

    }
}
