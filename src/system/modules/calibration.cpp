#include "system/modules/calibration.h"
#include "system/kernel/core.h"
#include "system/modules/orientation.h"
#include "system/modules/logger.h"
#include "system/kernel/calls.h"
#include "system/kernel/nvstorage.h"
#include "system/utils/events.h"
#include "system/utils/linkedlist.h"

const char* KEY_CALIBRATION_GIROSCOPE = "cal_giro";

Vector3D_t CalibrateGiroscope() {
    return CalibrateGiroscope(CalibrationConfig_t{
        .measureIntervalMs = 100,
        .measuringTimeS = 5
    });
}

Vector3D_t CalibrateGiroscope(CalibrationConfig_t config)
{
    //
    // The Giroscope calibration is done by let the device in a table without touching it.
    //
    // Setup timer to run a measure every 100ms, so we could calibrate using constant diff time. 
    //
    LinkedList<Vector3D_t> measures;
    OrientationData_t orientationData;
    esp_timer_handle_t updateSensorsTimer_h;
    TickType_t xLastWakeTime;
    TaskHandle_t self = xTaskGetCurrentTaskHandle ();

    esp_timer_create_args_t updateSensorTimerConfig = {
        .callback = [](TaskHandle_t self) -> void {
            logger.print('.');
            xTaskNotify(self, 0x00, eNoAction);
        },
        .arg = self,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "CalibrationUpdate"
    };

    logger << LOG_INFO << F("Measuring giroscope for 5s") << EndLine;

    esp_timer_create (&updateSensorTimerConfig, &updateSensorsTimer_h);
    esp_timer_start_once (updateSensorsTimer_h, config.measuringTimeS * 1000000UL);

    while (xTaskNotifyWait(0, 0, NULL, 0) != pdPASS) {
        xLastWakeTime = xTaskGetTickCount();
        vTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS(config.measureIntervalMs));

        OsKernel::OsCall(OS_SERVICE_UPDATE_ORIENTATION, &orientationData);
        measures.add(orientationData.rotation);
    }

    logger << EndLine;

    esp_timer_delete(updateSensorsTimer_h);

    Vector3D_t average = measures.reduce<Vector3D_t>([](Vector3D_t *prev, Vector3D_t current) -> void{
        prev->x += current.x;
        prev->y += current.y;
        prev->z += current.z;
    });
    average.x /= measures.size();
    average.y /= measures.size();
    average.z /= measures.size();

    measures.clear();

    return average;
}

Vector3D_t GetStoredGiroscopeCalibrations() {
    Blob_t offsetsData;
    bool status = OsKernel::NvVariableGet(KEY_CALIBRATION_GIROSCOPE, &offsetsData);
    if (!status) {
        logger << LOG_WARN << F("Can't load stored calibrations!") << EndLine;
    }
    return *((Vector3D_t*)offsetsData.pData);
}

bool StoreGiroscopeCalibrationResults(Vector3D_t calibratedOffsets) {
    return OsKernel::NvVariableSet(KEY_CALIBRATION_GIROSCOPE,
        Blob_t{.size = sizeof(calibratedOffsets), .pData = &calibratedOffsets});
}
