#include "system/drivers/battery.h"
#include "system/kernel/core.h"
#include "system/modules/logger.h"
#include "system/utils/sensor_state_str.h"
#include "system/utils/errors.h"

BatterySensor::BatterySensor(uint8_t pin)
    : pin(pin)
{ }

bool BatterySensor::init()
{
    logger << LOG_DEBUG << F("Initializing battery sensor... ");
    pinMode(pin, INPUT);
    this->checkState();
    switch (this->state)
    {
    case SENSOR_OK:
        logger << LOGGER_TEXT_GREEN << F("Ok") << EndLine;
        break;
    case SENSOR_ERROR_ON_CONNECT:
        logger << LOGGER_TEXT_RED << F("Battery read was unsuccesfull") << EndLine;
        break;
    default:
        logger << LOGGER_TEXT_YELLOW << F("UNKNOWN") << EndLine;
        break;
    }
    return this->state == SENSOR_OK;
}

bool BatterySensor::update()
{
    this->batteryLevel = this->readPercentage();
    return true;
}

void BatterySensor::dumpInfo()
{
    logger << LOG_DEBUG << F("Sensor BatteryADC:") << EndLine
            << LOG_DEBUG << F("\tState: ") << GetSensorStateStr(this->state) << EndLine
            << LOG_DEBUG << F("\tType: ADC, pin: ") << (uint8_t)this->pin << EndLine;

    if (this->firstReadDone) {
        logger << LOG_DEBUG << F("\tBatter Level: ") << this->batteryLevel << "%" << EndLine;
    }
}

SensorState_e BatterySensor::checkState ()
{
    uint16_t mv = readMilliVolts();
    if (mv < 2000) {
        return SENSOR_ERROR_ON_CONNECT;
    }
    return SENSOR_OK;
}

uint16_t BatterySensor::readMilliVolts ()
{
    uint16_t rawMeasure = analogRead (pin);
    uint16_t mv = ((rawMeasure * 6600) / 4095);
    return mv;
}

float BatterySensor::readPercentage ()
{
    uint16_t mv = this->readMilliVolts();
    float batPercent = ( 123.0f - ( 123.0f / pow( 1.0f + pow( (float)mv / 3700.0f, 80 ), 0.165 ) ) );
    return round(batPercent);
}