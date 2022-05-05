#if !defined(__CODE_GRAV_SYSTEM_UTILS_TYPES_H__)
#define __CODE_GRAV_SYSTEM_UTILS_TYPES_H__

#include <stdint.h>

typedef uint16_t Temperature_t;

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} Vector3D_t;

typedef struct {
    Vector3D_t acceleration;
    Vector3D_t rotation;
    Vector3D_t magnetometer;
} OrientationData_t;

typedef struct {
    OrientationData_t orientation;
    Temperature_t temperature;
} SystemData_t;

#endif // __CODE_GRAV_SYSTEM_UTILS_TYPES_H__
