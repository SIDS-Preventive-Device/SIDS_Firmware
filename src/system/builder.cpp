#include "system/builder.h"

#include <Arduino.h>
#include <Wire.h>

SystemBuilder::SystemBuilder()
{
    //
    // Set default configuration
    //
    this->config.baudrate = 115200;
    this->config.serialPort = &Serial;
    this->config.logLevel = LOG_INFO;
    this->config.i2cBus = &Wire;
    this->config.sensors = new LinkedList<ISensor*>();
    this->config.sensors->clear();
}

SystemBuilder &SystemBuilder::setSerialConfig(HardwareSerial &port, uint32_t baudrate)
{
    this->config.baudrate = baudrate;
    this->config.serialPort = &port;
    return *this;
}

SystemBuilder &SystemBuilder::setLogLevel(eLogLevel_t level)
{
    this->config.logLevel = level;
    return *this;
}

SystemBuilder &SystemBuilder::setI2Cport(TwoWire &bus)
{
    this->config.i2cBus = &bus;
    return *this;
}

SystemCore SystemBuilder::build()
{
    return SystemCore(this->config);
}