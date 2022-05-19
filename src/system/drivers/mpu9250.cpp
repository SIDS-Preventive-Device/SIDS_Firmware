#include "system/drivers/mpu9250.h"
#include "system/modules/logger.h"
#include "system/utils/sensor_state_str.h"
#include "system/utils/errors.h"
#include "system/utils/i2c.h"
#include "system/kernel/core.h"

#define __SENSOR_ADDRESS MPU9250_DEFAULT_ADDRESS

SensorMPU9250::SensorMPU9250()
    : device(__SENSOR_ADDRESS), firstReadDone(false)
{
}

bool SensorMPU9250::init() {
    logger << LOG_DEBUG << F("Initializing Sensor MPU9250") << EndLine;
    this->device.initialize();
    logger << LOG_DEBUG << F("Testing connection... ");
    this->checkState();
    switch (this->state)
    {
    case SENSOR_OK:
        logger << LOGGER_TEXT_GREEN << F("Ok") << EndLine;
        break;
    case SENSOR_NOT_FOUND:
        logger << LOGGER_TEXT_RED << F("Not found") << EndLine;
        break;
    case SENSOR_ERROR_ON_CONNECT:
        logger << LOGGER_TEXT_RED << F("Error to connect") << EndLine;
        break;
    default:
        logger << LOGGER_TEXT_YELLOW << F("UNKNOWN") << EndLine;
        break;
    }
    return this->state == SENSOR_OK;
}

SensorState_e SensorMPU9250::checkState () {
    this->state = SENSOR_OK;
    if (!isI2CDeviceConnected(*OsKernel::OsGetSysConfigPtr()->i2cBus, __SENSOR_ADDRESS)) {
        this->state = SENSOR_NOT_FOUND;
    }
    if (!this->device.testConnection()) {
        this->state = SENSOR_ERROR_ON_CONNECT;
    }

    return this->state;
}

bool SensorMPU9250::update() {
    if (!this->firstReadDone) { this->firstReadDone = true; }

    Vector3D_t *pAcceleration = &this->acc;
    Vector3D_t *pGiroscope = &this->giro;
    Vector3D_t *pMagnetometer = &this->mag;

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
            << LOG_DEBUG << F("\tType: I2C, address: 0x") << INT_HEX << (uint8_t)__SENSOR_ADDRESS << EndLine;

    if (this->firstReadDone) {
        Vector3D_t *pAcceleration = &this->acc;
        Vector3D_t *pGiroscope = &this->giro;
        Vector3D_t *pMagnetometer = &this->mag;

        logger << LOG_DEBUG << F("\tMeasures:") << EndLine
                << LOG_DEBUG << F("\tAx: ") << pAcceleration->x << F("\tAy: ") << pAcceleration->y << F("\tAz: ") << pAcceleration->z << EndLine
                << LOG_DEBUG << F("\tGx: ") << pGiroscope->x << F("\tGy: ") << pGiroscope->y << F("\tGz: ") << pGiroscope->z << EndLine
                << LOG_DEBUG << F("\tMx: ") << pMagnetometer->x << F("\tMy: ") << pMagnetometer->y << F("\tMz: ") << pMagnetometer->z << EndLine;
    }
}