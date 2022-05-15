#if !defined(__CODE_GRAV_SYSTEM_UTILS_TYPES_H__)
#define __CODE_GRAV_SYSTEM_UTILS_TYPES_H__

#include <stdint.h>
#include <Printable.h>
#include <Print.h>

typedef uint16_t Temperature_t;

struct Vector3D_t : Printable {
    int16_t x;
    int16_t y;
    int16_t z;

    virtual size_t printTo(Print& p) const {
        return p.printf("[%04i %04i %04i]", x, y, z);
    };
};

typedef struct Vector3D_t Vector3D_t;

typedef struct {
    Vector3D_t acceleration;
    Vector3D_t rotation;
    Vector3D_t magnetometer;
} OrientationData_t;

#endif // __CODE_GRAV_SYSTEM_UTILS_TYPES_H__
