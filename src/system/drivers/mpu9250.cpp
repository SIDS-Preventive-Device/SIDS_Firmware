#include "system/drivers/mpu9250.h"
#include "system/modules/logger.h"
#include "system/utils/sensor_state_str.h"
#include "system/utils/errors.h"
#include "system/utils/i2c.h"
#include "system/kernel/core.h"

#define __SENSOR_ADDRESS 0x68

SensorMPU9250::SensorMPU9250()
    : device(i2c0, __SENSOR_ADDRESS), firstReadDone(false)
{
}

bool SensorMPU9250::init() {
    logger << LOG_DEBUG << F("Initializing Sensor MPU9250") << EndLine;
    if (this->device.begin() < 0) {
        logger << LOG_ERROR << "Can't begin MPU" << EndLine;
        this->state = SENSOR_ERROR_ON_CONNECT;
    }

    this->device.setAccelRange(MPU9250::ACCEL_RANGE_2G);
    this->device.setGyroRange(MPU9250::GYRO_RANGE_250DPS);
    this->device.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_10HZ);

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
    if (OsKernel::OsGetSysConfigPtr()->i2cBus->testConnection(__SENSOR_ADDRESS)) {
        this->state = SENSOR_NOT_FOUND;
    }
    return this->state;
}

bool SensorMPU9250::update() {
    if (!this->firstReadDone) { this->firstReadDone = true; }

    this->device.readSensor();
    this->acc.x  = this->device.getAccelX_mss();
    this->acc.y  = this->device.getAccelY_mss();
    this->acc.z  = this->device.getAccelZ_mss();
    this->giro.x = this->device.getGyroX_rads();
    this->giro.y = this->device.getGyroX_rads();
    this->giro.z = this->device.getGyroX_rads();
    this->mag.x  = this->device.getMagX_uT();
    this->mag.y  = this->device.getMagY_uT();
    this->mag.z  = this->device.getMagZ_uT();

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