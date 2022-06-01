#if !defined(__CODE_GRAV_SYSTEM_BUILDER_H__)
#define __CODE_GRAV_SYSTEM_BUILDER_H__

#include "core.h"
#include "system/modules/logger.h"
#include <I2Cdev.h>

class SystemBuilder {
    SystemConfig_t config;
public:
    SystemBuilder ();

    SystemBuilder &setSerialConfig(HardwareSerial &port, uint32_t baudrate);

    SystemBuilder &setLogLevel(eLogLevel_t level);

    SystemBuilder &setI2Cport(i2cbus::I2C &bus);

    SystemBuilder &enableBLE(const __FlashStringHelper* bleDeviceName);

    template <class ISensorType>
    SystemBuilder &setOrientationSensor() {
        config.orientationSensor = new ISensorType();
        return *this;
    }

    template <class ISensorType>
    SystemBuilder &setBatterySensor() {
        config.batterySensor = new ISensorType();
        return *this;
    }

    template <class ISensorType>
    SystemBuilder &setTemperatureSensor() {
        config.temperatureSensor = new ISensorType();
        return *this;
    }

    template <class ISensorType>
    SystemBuilder &setRiskAlert() {
        config.riskAlert = new ISensorType();
        return *this;
    }

    SystemConfig_t build();
};

#endif // __CODE_GRAV_SYSTEM_BUILDER_H__
