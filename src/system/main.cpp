#include "system/kernel/core.h"
#include "system/utils/types.h"
#include "system/modules/orientation.h"
#include "system/modules/logger.h"

KERNEL_BOOT_THREAD_FUNC(BOOT_NORMAL) {
    logger << LOG_MASTER << F("Normal mode enabled!") << EndLine;

    while (true) {
    }
}

KERNEL_BOOT_THREAD_FUNC(BOOT_CALIBRATION) {
    logger << LOG_MASTER << F("Calibration mode enabled!") << EndLine;

    while (true) {

    }
}
