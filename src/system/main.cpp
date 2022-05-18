#include "system/kernel/core.h"
#include "system/utils/types.h"
#include "system/modules/orientation.h"
#include "system/modules/calibration.h"
#include "system/modules/logger.h"
#include "system/kernel/calls.h"
#include "system/kernel/nvstorage.h"
#include "system/utils/events.h"
#include "system/utils/linkedlist.h"

#include <math.h>

//
// gyro default 250 LSB per d/s -> rad/s
//
#define gscale (PI/180.0)

#define INTERVAL_DEBUG 500
#define INTERVAL_PRODUCTION 30

/**
 * @brief Construct a new kernel boot thread this is the entry point for
 *        boot mode normal.
 * 
 */
KERNEL_BOOT_THREAD_FUNC(BOOT_NORMAL) {
    TickType_t lastTicks = xTaskGetTickCount();
    Matrix<3, 1, int16_t> giroscopeOffsets = VariableCalibrationNvram<Vector3D_t>::Restore(GiroscopeCalibration).toMatrix();
    Matrix<3, 1, int16_t> accOffsets = VariableCalibrationNvram<Vector3D_t>::Restore(AccelerometerCalibration).toMatrix();
    Matrix<3, 1, int16_t> dummyOffsets;
    Matrix<3, 3, float> dummyCorrection;
    dummyCorrection += 1.0f;

    OrientationParams_t params = {
        .giroscopeOffsets = giroscopeOffsets,
        .giroScale = gscale,
        .accelerometerOffsets = accOffsets,
        .accelerometerCorrection = dummyCorrection,
        .magnetometerOffsets = dummyOffsets,
        .magnetometerCorrection = dummyCorrection
    };

    logger << LOG_MASTER << ~giroscopeOffsets << EndLine;
    logger << LOG_MASTER << ~accOffsets << EndLine;

    unsigned long startTc;
    unsigned long endTc;

    while (true) {
        startTc = micros();
        CalculateOrientation(params);
        endTc = micros();
        logger << LOG_INFO << F("CalculateOrientation - Execution time: ") << (uint32_t)(endTc - startTc) << F("us") << EndLine;

        vTaskDelayUntil(&lastTicks, pdMS_TO_TICKS(INTERVAL_DEBUG));
    }
}

/**
 * @brief Construct a new kernel boot thread this is the entry point for
 *        boot mode calibration.
 * 
 */
KERNEL_BOOT_THREAD_FUNC(BOOT_CALIBRATION) {
    //
    // Calibrate giroscope.
    //
    AverageOffsetsCalibrationsConfig_t<Vector3D_t> GConfig = {
        .measureIntervalMs = 100,
        .measuringTimeS = 5,
        .pContext = NULL,
        .preMeasures = NULL,
        .onMeasure = [](void* pContext) -> Vector3D_t {
            OrientationData_t data;
            OsKernel::OsCall(OS_SERVICE_UPDATE_ORIENTATION, &data);
            return data.rotation;
        },
        .postMeasures = NULL,
        .processResults = [](void *pContext, LinkedList<Vector3D_t>* measures) -> Vector3D_t {
            Vector3D_t average = measures->reduce<Vector3D_t>([](Vector3D_t *prev, Vector3D_t current) -> void{
                prev->x += current.x;
                prev->y += current.y;
                prev->z += current.z;
            });
            average.x /= measures->size();
            average.y /= measures->size();
            average.z /= measures->size();
            return average;
        }
    };
    AverageOffsetsCalibrationsConfig_t<Vector3D_t> AConfig = {
        .measureIntervalMs = 100,
        .measuringTimeS = 10,
        .pContext = NULL,
        .preMeasures = NULL,
        .onMeasure = [](void* pContext) -> Vector3D_t {
            OrientationData_t data;
            OsKernel::OsCall(OS_SERVICE_UPDATE_ORIENTATION, &data);
            return data.acceleration;
        },
        .postMeasures = NULL,
        .processResults = [](void *pContext, LinkedList<Vector3D_t>* measures) -> Vector3D_t {
            Vector3D_t average = measures->reduce<Vector3D_t>([](Vector3D_t *prev, Vector3D_t current) -> void{
                prev->x += current.x;
                prev->y += current.y;
                prev->z += current.z;
            });
            average.x /= measures->size();
            average.y /= measures->size();
            average.z /= measures->size();
            return average;
        }
    };

    GiroscopeCalibration.setConfig(GConfig);
    GiroscopeCalibration.calibrate();
    logger << LOG_INFO << F("Giroscope calibration values: ") << LOGGER_TEXT_YELLOW << GiroscopeCalibration.getResults() << EndLine;

    AccelerometerCalibration.setConfig(AConfig);
    AccelerometerCalibration.calibrate();
    logger << LOG_INFO << F("Accelerometer calibration values: ") << LOGGER_TEXT_YELLOW << AccelerometerCalibration.getResults() << EndLine;

    VariableCalibrationNvram<Vector3D_t>::Save(GiroscopeCalibration);
    VariableCalibrationNvram<Vector3D_t>::Save(AccelerometerCalibration);
    logger << LOG_INFO << F("Results stored!") << EndLine;

    OsKernel::OsCall(OS_SERVICE_REBOOT, NULL);
}
