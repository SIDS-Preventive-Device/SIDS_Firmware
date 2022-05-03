#if !defined(__CODE_GRAV_SYSTEM_BUILDER_H__)
#define __CODE_GRAV_SYSTEM_BUILDER_H__

#include "core.h"

class SystemBuilder {
    SystemConfig_t config;
public:
    SystemBuilder ();

    template <class Tsensor>
    SystemBuilder &addSensor ()
    {
        this->config.sensors->add(new Tsensor());
        return *this;
    }

    SystemBuilder &setSerialConfig(HardwareSerial &port, uint32_t baudrate);

    SystemBuilder &setLogLevel(eLogLevel_t level);

    SystemBuilder &setI2Cport(TwoWire &bus);

    SystemCore build();
};

#endif // __CODE_GRAV_SYSTEM_BUILDER_H__
