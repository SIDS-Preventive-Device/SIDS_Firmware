#include "system/utils/sensor_state_str.h"
#include <system/utils/array_size.h>

const __FlashStringHelper* SensorStateStrMap[SENSOR_DELIMITER] = {
    F("OK"),
    F("ERROR"),
    F("NOT FOUND"),
    F("CALIBRATION")
};

const __FlashStringHelper* GetSensorStateStr (SensorState_e state)
{
    if (state >= ARRAY_SIZE(SensorStateStrMap)) {
        return F("UNKNOWN");
    }
    return SensorStateStrMap[(uint8_t)state];
}