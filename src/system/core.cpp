/*
--------------------------------------------------------
   _____             _        _____                     
  /  __ \           | |      |  __ \                    
  | /  \/  ___    __| |  ___ | |  \/ _ __   __ _ __   __
  | |     / _ \  / _` | / _ \| | __ | '__| / _` |\ \ / /
  | \__/\| (_) || (_| ||  __/| |_\ \| |   | (_| | \ V / 
   \____/ \___/  \__,_| \___| \____/|_|    \__,_|  \_/  
--------------------------------------------------------
*/

#include "system/core.h"
#include "system/errors.h"
#include "system/logger.h"
#include "system/utils/i2c.h"
#include "system/types.h"

#include <Arduino.h>
#include <Wire.h>

#define FIRMWARE_VERSION F("0.1.1")

KernelLogger logger (nullptr);

SystemCore::SystemCore (SystemConfig_t config)
    : configuration(config)
{
    memset(&this->measures, 0x00, sizeof(this->measures));
}

SystemCore::~SystemCore () {
    uint8_t index;
    if (this->configuration.sensors != nullptr) {
        for (index = 0; index < this->configuration.sensors->size(); index++) {
            delete this->configuration.sensors->get(index);
        }
    }
    delete this->configuration.sensors;
}

SystemCore &SystemCore::init () {
    this->initializeLogger();
    this->initializeBuses();
    this->dumpSysInfo();
    this->initializeSensors();
    logger << LOG_MASTER << LOGGER_TEXT_GREEN
        << F("Initialization finished!")
        << EndLine;
    return *this;
}

void SystemCore::execute () {
    logger << LOG_MASTER << LOGGER_TEXT_GREEN
        << F("Running kernel")
        << EndLine;

    while (1) {
        ;
    }
}

void SystemCore::initializeBuses () {
    if (this->configuration.serialPort != nullptr) {
        this->configuration.serialPort->begin(
            this->configuration.baudrate
            );
    } else {
        LogKernelError ("Serial Bus instance is NULL!!!");
    }

    if (this->configuration.i2cBus != nullptr) {
        this->configuration.i2cBus->begin();
    } else {
        LogKernelError ("I2C Bus instance is NULL!!!");
    }

}

void SystemCore::initializeLogger () {
    logger.setLogOutput(this->configuration.serialPort);
    logger.setLogLevel(this->configuration.logLevel);
}

void SystemCore::initializeSensors () {
    const uint8_t SensorsMax = this->configuration.sensors->size();
    uint8_t index;
    ISensor *pSensor;
    
    logger << LOG_INFO << F("Initializing sensors, detected ") << SensorsMax << EndLine;
    
    for (index = 0; index < SensorsMax; index ++) {
        pSensor = this->configuration.sensors->get(index);
        if (pSensor == nullptr) {
            LogKernelError ("\tWhile reading sensor list, found sensor ptr as NULL!");
            continue;
        }
        if (!pSensor->init(this)) {
            LogMinorError ("\tError initializing a sensor!");
        }
        pSensor->dumpInfo();
    }
}

void SystemCore::dumpSysInfo () {
    logger << LOG_MASTER << EndLine << LOGGER_TEXT_BOLD << LOGGER_TEXT_YELLOW <<
    F("--------------------------------------------------------\n") <<
    F("   _____             _        _____                     \n") <<
    F("  /  __ \\           | |      |  __ \\                    \n") <<
    F("  | /  \\/  ___    __| |  ___ | |  \\/ _ __   __ _ __   __\n") <<
    F("  | |     / _ \\  / _` | / _ \\| | __ | '__| / _` |\\ \\ / /\n") <<
    F("  | \\__/\\| (_) || (_| ||  __/| |_\\ \\| |   | (_| | \\ V / \n") <<
    F("   \\____/ \\___/  \\__,_| \\___| \\____/|_|    \\__,_|  \\_/  \n") <<
    F("--------------------------------------------------------\n") <<
    LOG_MASTER << F("Fimware version: ") << FIRMWARE_VERSION << EndLine <<
    LOG_MASTER << F("Build date: ") << __DATE__ << F(" ") << __TIME__ << EndLine << EndLine;

    DumpI2Cdevices(*this->configuration.i2cBus);
}