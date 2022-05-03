#include "system/drivers/mpu9250.h"
#include "system/logger.h"
#include "system/utils/sensor_state_str.h"

#define __SENSOR_ADDRESS MPU9250_DEFAULT_ADDRESS

SensorMPU9250::SensorMPU9250()
    : device(__SENSOR_ADDRESS), id(0)
{}

bool SensorMPU9250::init() {
    logger << LOG_DEBUG << F("Initializing SensorMPU9250") << EndLine;
    this->device.initialize();
    this->state = SENSOR_OK;
    logger << LOG_DEBUG << F("Testing connection...") << EndLine;
    this->id = this->device.getDeviceID();
    if (!this->device.testConnection()) {
        logger << LOG_DEBUG << F("device.testConnection was false!") << EndLine;
        this->state = SENSOR_ERROR_ON_CONNECT;
        return false;
    }
    return true;
}

bool SensorMPU9250::update() {
    return true;
}

void SensorMPU9250::dumpInfo() {
    logger << LOG_DEBUG << F("Dump info SensorMPU9250 ------") << EndLine
            << LOG_DEBUG << F("\tState: ") << GetSensorStateStr(this->state) << EndLine
            << LOG_DEBUG << F("\tType: I2C, address: 0x") << INT_HEX << (uint8_t)__SENSOR_ADDRESS << EndLine
            << LOG_DEBUG << F("\tID: ") << this->id << EndLine;
}