#if !defined(__CODE_GRAV_SYSTEM_UTILS_TYPES_H__)
#define __CODE_GRAV_SYSTEM_UTILS_TYPES_H__

#include <stdint.h>
#include <Printable.h>
#include <Print.h>
#include "matrix.h"

typedef uint16_t Temperature_t;

struct Vector3D_t : Printable {
    int16_t x;
    int16_t y;
    int16_t z;

    virtual size_t printTo(Print& p) const {
        return p.printf("[%4d %4d %4d]", x, y, z);
    };

    Matrix<3, 1, int16_t>  toMatrix() {
        Matrix<3, 1, int16_t>  vec;
        vec[0][0] = x;
        vec[1][0] = y;
        vec[2][0] = z;
        return vec;
    }

    static Vector3D_t zero() {
        Vector3D_t vec;
        vec.x = 0;
        vec.y = 0;
        vec.z = 0;
        return vec;
    }
};

typedef struct Vector3D_t Vector3D_t;

typedef struct {
    Vector3D_t acceleration;
    Vector3D_t rotation;
    Vector3D_t magnetometer;
} OrientationData_t;

class IRiskAlert {
public:
    virtual void Configure () = 0;
    virtual void Alert(uint32_t milliseconds = 0) const = 0;
};

#endif // __CODE_GRAV_SYSTEM_UTILS_TYPES_H__
