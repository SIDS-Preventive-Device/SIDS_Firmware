#if !defined(__CODE_GRAV_SYSTEM_DEF_INTERFACES_H__)
#define __CODE_GRAV_SYSTEM_DEF_INTERFACES_H__

#include "utils/linkedlist.h"
#include <stdint.h>

typedef enum {
    VARIABLE_ACCELERATION,
    VARIABLE_GIROSCOPE,
    VARIABLE_MAGNETOMETER,
    VARIABLE_TEMPERATURE,
    VARIABLE_DELIMITER
} Variable_e;

typedef enum {
    SENSOR_OK,
    SENSOR_ERROR_ON_CONNECT,
    SENSOR_NOT_FOUND,
    SENSOR_CALIBRATION,
    SENSOR_DELIMITER
} SensorState_e;

class ISensor {
protected:
    LinkedList<Variable_e> variables;
    SensorState_e state;

public:
    virtual bool init (void *pSystemCore) = 0;
    virtual bool update () = 0;
    virtual void dumpInfo () = 0;

    template <typename Tvar>
    bool haveVariable () {
        uint8_t index;
        for (index = 0; index < variables.size(); index++) {
            if (Tvar == variables[index].variable) {
                return true;
            }
        }
        return false;
    };

    virtual ~ISensor() {}
};

#endif // __CODE_GRAV_SYSTEM_DEF_INTERFACES_H__
