#include "system/kernel/core.h"
#include "system/utils/types.h"
#include "system/modules/orientation.h"
#include "system/modules/calibration.h"
#include "system/modules/logger.h"
#include "system/kernel/calls.h"
#include "system/kernel/nvstorage.h"
#include "system/utils/events.h"
#include "system/utils/linkedlist.h"

#include <BluetoothSerial.h>

#include <math.h>

//
// gyro default 250 LSB per d/s -> rad/s
//
#define gscale (250.0/32768.0)*(PI/180.0)

/**
 * @brief Construct a new kernel boot thread this is the entry point for
 *        boot mode normal.
 * 
 */
KERNEL_BOOT_THREAD_FUNC(BOOT_NORMAL) {
    logger << LOG_MASTER << F("Normal mode enabled!") << EndLine;
    OrientationData_t orientationData;

    Matrix<3, 1, int16_t> giroscopeOffsets = fromVector3D_t(GetStoredGiroscopeCalibrations());

    while (true) {
        OsKernel::OsCall(OS_SERVICE_UPDATE_ORIENTATION, &orientationData);
        Matrix<3, 1, int16_t> rsult = (fromVector3D_t(orientationData.rotation) + giroscopeOffsets) * gscale;

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
    Vector3D_t giroscopeOffsets;

    //
    // Calibrate giroscope.
    //
    giroscopeOffsets = CalibrateGiroscope();
    StoreGiroscopeCalibrationResults (giroscopeOffsets);
    logger << LOG_INFO << F("Giroscope calibration values: ") << LOGGER_TEXT_YELLOW << giroscopeOffsets << EndLine;

    OsKernel::OsCall(OS_SERVICE_REBOOT, NULL);
}
