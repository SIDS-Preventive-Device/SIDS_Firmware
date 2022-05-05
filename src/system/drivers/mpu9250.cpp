#include "system/drivers/mpu9250.h"
#include "system/logger.h"
#include "system/utils/sensor_state_str.h"
#include "system/errors.h"

#define __SENSOR_ADDRESS MPU9250_DEFAULT_ADDRESS

SensorMPU9250::SensorMPU9250()
    : device(__SENSOR_ADDRESS), id(0), outputData(NULL), firstReadDone(false)
{
    this->variables.clear();
    this->variables.add(VARIABLE_ACCELERATION);
    this->variables.add(VARIABLE_GIROSCOPE);
    this->variables.add(VARIABLE_MAGNETOMETER);
}

bool SensorMPU9250::init(void *pSystemCoreData) {
    logger << LOG_DEBUG << F("Initializing Sensor MPU9250") << EndLine;
    this->device.initialize();
    this->state = SENSOR_OK;
    logger << LOG_DEBUG << F("Testing connection...") << EndLine;
    this->id = this->device.getDeviceID();
    if (!this->device.testConnection()) {
        logger << LOG_DEBUG << F("device.testConnection was false!") << EndLine;
        this->state = SENSOR_ERROR_ON_CONNECT;
        return false;
    }
    if (pSystemCoreData == nullptr) {
        LogMinorError ("pSystemCoreData is NULL!");;
        return false;
    }
    outputData = &((SystemData_t*)pSystemCoreData)->orientation;
    return true;
}

bool SensorMPU9250::update() {
    if (outputData == nullptr) {
        LogMinorError ("Sensor output data ptr is not defined!");
        return false;
    }

    if (!this->firstReadDone) { this->firstReadDone = true; }

    Vector3D_t *pAcceleration = &this->outputData->acceleration;
    Vector3D_t *pGiroscope = &this->outputData->rotation;
    Vector3D_t *pMagnetometer = &this->outputData->magnetometer;

    this->device.getMotion9(
        &pAcceleration->x,
        &pAcceleration->y,
        &pAcceleration->z,
        &pGiroscope->x,
        &pGiroscope->y,
        &pGiroscope->z,
        &pMagnetometer->x,
        &pMagnetometer->y,
        &pMagnetometer->z
    );

    return true;
}

void SensorMPU9250::dumpInfo() {
    logger << LOG_DEBUG << F("Sensor MPU9250:") << EndLine
            << LOG_DEBUG << F("\tState: ") << GetSensorStateStr(this->state) << EndLine
            << LOG_DEBUG << F("\tType: I2C, address: 0x") << INT_HEX << (uint8_t)__SENSOR_ADDRESS << EndLine
            << LOG_DEBUG << F("\tID: ") << this->id << EndLine;

    if (this->outputData && this->firstReadDone) {
        Vector3D_t *pAcceleration = &this->outputData->acceleration;
        Vector3D_t *pGiroscope = &this->outputData->rotation;
        Vector3D_t *pMagnetometer = &this->outputData->magnetometer;

        logger << LOG_DEBUG << F("\tMeasures:") << EndLine
                << LOG_DEBUG << F("\tAx: ") << pAcceleration->x << F("\tAy: ") << pAcceleration->y << F("\tAz: ") << pAcceleration->z << EndLine
                << LOG_DEBUG << F("\tGx: ") << pGiroscope->x << F("\tGy: ") << pGiroscope->y << F("\tGz: ") << pGiroscope->z << EndLine
                << LOG_DEBUG << F("\tMx: ") << pMagnetometer->x << F("\tMy: ") << pMagnetometer->y << F("\tMz: ") << pMagnetometer->z << EndLine;
    }
}