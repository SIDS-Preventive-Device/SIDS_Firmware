#if !defined(__CODE_GRAV_SYSTEM_UTILS_I2C_H__)
#define __CODE_GRAV_SYSTEM_UTILS_I2C_H__

#include <Wire.h>

extern bool isI2CDeviceConnected(TwoWire &bus, uint8_t address);
extern void DumpI2Cdevices(TwoWire &bus);

#endif // __CODE_GRAV_SYSTEM_UTILS_I2C_H__
