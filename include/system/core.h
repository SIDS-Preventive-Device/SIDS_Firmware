#if !defined(__CODE_GRAV_SYSTEM_CORE_H__)
#define __CODE_GRAV_SYSTEM_CORE_H__

#include <stdint.h>
#include "logger.h"
#include "isensor.h"
#include "utils/linkedlist.h"

class TwoWire;
class HardwareSerial;

typedef struct {
    uint32_t baudrate;
    HardwareSerial *serialPort;
    eLogLevel_t logLevel;

    TwoWire *i2cBus;

    LinkedList<ISensor*> *sensors;
} SystemConfig_t;

class SystemCore {
    SystemConfig_t configuration;

public:
    SystemCore (SystemConfig_t config);
    SystemCore &init ();
    void execute ();

    ~SystemCore ();
private:
    void initializeBuses ();
    void initializeLogger ();
    void initializeSensors ();

    void dumpSysInfo ();
};

#endif // __CODE_GRAV_SYSTEM_CORE_H__
