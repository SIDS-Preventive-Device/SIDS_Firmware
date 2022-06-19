#include "system/kernel/core.h"
#include "system/utils/types.h"
#include "system/modules/orientation.h"
#include "system/modules/calibration.h"
#include "system/modules/logger.h"
#include "system/kernel/calls.h"
#include "system/kernel/nvstorage.h"
#include "system/utils/events.h"
#include "system/utils/linkedlist.h"
#include "system/kernel/ble.h"

#include <math.h>

//
// gyro default 250 LSB per d/s -> rad/s
//
#define gscale ((250.0/32768.0)*(PI/180.0))

#define INTERVAL_DEBUG 500
#define INTERVAL_PRODUCTION 100

#define MEASURE_INTERVAL INTERVAL_PRODUCTION
#define POST_ALERT_WAIT  1000

#define RISK_THRESHOLD 0

/**
 * @brief Construct a new kernel boot thread this is the entry point for
 *        boot mode normal.
 * 
 */
KERNEL_BOOT_THREAD_FUNC(BOOT_NORMAL) {
    TickType_t lastTicks = xTaskGetTickCount();
    OrientationData_t orientationData;
    EulerMatrix_t results;
    OrientationParams_t params;
    uint8_t calculatedRisk;

    params = {
        .giroScale = gscale,
        .measureInterval = MEASURE_INTERVAL
    };

    while (true) {
        //
        // Update orientation sensor
        //
        OsKernel::OsCall(OS_SERVICE_UPDATE_ORIENTATION, &orientationData);

        //
        // Calculate quaternion values from orientation sensor using the parameters defined.
        //
        results = CalculateOrientation(orientationData, params);

        //
        // Determine the risk of the position
        //
        calculatedRisk = CalculatePositionRisk(results);

        //
        // Current risk is important enough?
        //
        if (calculatedRisk > RISK_THRESHOLD) {
            //
            // Alert and activate buzzer!
            //
            OsKernel::OsCall(OS_SERVICE_THROW_POSITION_RISK_ALERT, NULL);
            vTaskDelayUntil(&lastTicks, pdMS_TO_TICKS(POST_ALERT_WAIT));
            continue;
        }

        //
        // Update temperature
        //
        OsKernel::OsCall(OS_SERVICE_UPDATE_TEMPERATURE, NULL);

        vTaskDelayUntil(&lastTicks, pdMS_TO_TICKS(MEASURE_INTERVAL));
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
            Vector3D_t average = measures->reduce<Vector3D_t>([](Vector3D_t *prev, Vector3D_t current) -> void {
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

    //
    // Calibrate accelerometer configuration
    //
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

    VariableCalibrationNvram<Vector3D_t>::Save(GiroscopeCalibration);
    logger << LOG_INFO << F("Results stored!") << EndLine;

    OsKernel::OsCall(OS_SERVICE_REBOOT, NULL);
}
