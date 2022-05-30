#include "../kernel_internal.h"
#include "system/utils/errors.h"
#include "system/modules/logger.h"

KERNEL_CALLBACK(UPDATE_SERVICES_REFRESH_SENSOR_STATE) {
    logger << LOG_INFO << F("Refresh sensor connections") << EndLine;
}

KERNEL_CALLBACK(UPDATE_SERVICES_BATTERY) {
    SystemConfig_t *config = OsKernel::OsGetSysConfigPtr();
    if (config->batterySensor->checkState() != SENSOR_OK) {
        return;
    }

    config->batterySensor->update();
    float raw = config->batterySensor->getRawMeasure();
    uint8_t level = (uint8_t)config->batterySensor->getBatteryLevel();
    logger << LOG_DEBUG << F("Battery Level: ") << level << F("%, mv: ") << raw << EndLine;

    OsKernel::SetBLECharacteristicValue(BLE_CHT_BATTERY, level);
}

KERNEL_SERVICE(OS_SERVICE_UPDATE_ORIENTATION) {
    SystemConfig_t *config = OsKernel::OsGetSysConfigPtr();
    OrientationData_t *pData = (OrientationData_t*)pContext;
    bool success = false;

    if (pData == nullptr) {
        logger << LOG_WARN << F("Can't update any sensors, no output buffer specified!") << EndLine;
        return;
    }

    success = config->orientationSensor->update();
    if (!success) {
        LogKernelError("Error on updating orientation sensor!");
        return;
    }

    pData->acceleration  = config->orientationSensor->getAccVector();
    pData->rotation      = config->orientationSensor->getGiroVector();
    pData->magnetometer  = config->orientationSensor->getMagVector();
}

KERNEL_SERVICE(OS_SERVICE_UPDATE_TEMPERATURE) {
}