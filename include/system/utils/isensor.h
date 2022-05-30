#if !defined(__CODE_GRAV_SYSTEM_DEF_INTERFACES_H__)
#define __CODE_GRAV_SYSTEM_DEF_INTERFACES_H__

#include <stdint.h>
#include "types.h"

typedef enum {
    SENSOR_OK,
    SENSOR_ERROR_ON_CONNECT,
    SENSOR_NOT_FOUND,
    SENSOR_CALIBRATION,
    SENSOR_DELIMITER
} SensorState_e;

class ISensor {
protected:
    SensorState_e state;
    String name;

public:
    void setName (const char* name) { 
        this->name = name;
    }

    const char* getName () const { 
        return this->name.c_str();
    }

    virtual bool init () = 0;
    virtual bool update () = 0;
    virtual SensorState_e checkState () = 0;
    virtual void dumpInfo () = 0;

    virtual ~ISensor() {}
};

class IOrientationSensor : public ISensor {
protected:
    Vector3D_t acc;
    Vector3D_t giro;
    Vector3D_t mag;
public:
    Vector3D_t getAccVector() const { return acc; }
    Vector3D_t getGiroVector() const { return giro; }
    Vector3D_t getMagVector() const { return mag; }
};

class IBatterySensor : public ISensor {
protected:
    float batteryLevel;
public:
    float getBatteryLevel() const { return batteryLevel; }
    virtual float getRawMeasure() = 0;
};

class ITemperatureSensor : public ISensor {
protected:
    uint8_t temperatureLevel;
public:
    uint8_t getTemperatureLevel() const { return temperatureLevel; }
};

#endif // __CODE_GRAV_SYSTEM_DEF_INTERFACES_H__
