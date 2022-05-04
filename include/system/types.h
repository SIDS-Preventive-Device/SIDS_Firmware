#if !defined(__CODE_GRAV_SYSTEM_TYPES_H__)
#define __CODE_GRAV_SYSTEM_TYPES_H__

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "system/utils/array_size.h"
#if !defined(ARDUINO) && defined(UNITY)
#include <iostream>
#endif

typedef uint16_t Temperature_t;

typedef struct {
    uint16_t x;
    uint16_t y;
    uint16_t z;
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

template <size_t sizeA, size_t sizeB, typename Tarray>
class Matrix
#if defined(ARDUINO)
    : public Printable
#endif
{
public:
    Tarray array[sizeA][sizeB];

public:
    Matrix() {
        size_t indexA = 0;
        size_t indexB = 0;
        for (indexA = 0; indexA < sizeA; indexA++) {
            for (indexB = 0; indexB < sizeB; indexB++) {
                array[indexA][indexB] = 0;
            }
        }
    }

    inline Tarray* operator[](int index) {
        return array[index];
    }
    inline Tarray* operator[](size_t &i) {
        return array[i];
    }
    inline const Tarray* operator[](const size_t &i) const {
        return array[i];
    }

    Matrix<sizeA, sizeB, Tarray> operator +(Matrix const &matrix) {
        Matrix<sizeA, sizeB, Tarray> result;
        size_t indexA = 0;
        size_t indexB = 0;

        for (indexA = 0; indexA < sizeA; indexA++) {
            for (indexB = 0; indexB < sizeB; indexB++) {
                result[indexA][indexB] = matrix[indexA][indexB] + array[indexA][indexB];
            }
        }

        return result;
    }

    Matrix<sizeA, sizeB, Tarray> operator +(int const &constant) {
        Matrix<sizeA, sizeB, Tarray> result;
        size_t indexA = 0;
        size_t indexB = 0;

        for (indexA = 0; indexA < sizeA; indexA++) {
            for (indexB = 0; indexB < sizeB; indexB++) {
                result[indexA][indexB] = array[indexA][indexB] + constant;
            }
        }
        
        return result;
    }

    Matrix<sizeA, sizeB, Tarray> operator *(int const &constant) {
        Matrix<sizeA, sizeB, Tarray> result;
        size_t indexA = 0;
        size_t indexB = 0;

        for (indexA = 0; indexA < sizeA; indexA++) {
            for (indexB = 0; indexB < sizeB; indexB++) {
                result[indexA][indexB] = array[indexA][indexB] * constant;
            }
        }
        
        return result;
    }

#if defined(ARDUINO)
    size_t printTo(Print& p) const {
        p.println();
        size_t indexA = 0;
        for (indexA = 0; indexA < sizeA; indexA++) {
            for (indexB = 0; indexB < sizeB; indexB++) {
                p.print(String(array[indexA][indexB]));
                p.print(F(" "));
            }
            p.println();
        }
    }
#endif

#if !defined(ARDUINO) && defined(UNITY)
    void dump() {
        size_t indexA = 0;
        size_t indexB = 0;
        for (indexA = 0; indexA < sizeA; indexA++) {
            for (indexB = 0; indexB < sizeB; indexB++) {
                std::cout << array[indexA][indexB] << " ";
            }
            std::cout << "\n";
        }
    }
#endif
};

template <size_t sizeA, size_t sizeB, typename Tarray>
Matrix<sizeA, sizeB, Tarray> operator *=(Matrix<sizeA, sizeB, Tarray> &matrix, int const &constant) {
    size_t indexA = 0;
    size_t indexB = 0;

    for (indexA = 0; indexA < sizeA; indexA++) {
        for (indexB = 0; indexB < sizeB; indexB++) {
            matrix.array[indexA][indexB] *= constant;
        }
    }
    
    return matrix;
}

template <size_t sizeA, size_t sizeB, typename Tarray>
Matrix<sizeA, sizeB, Tarray> operator +=(Matrix<sizeA, sizeB, Tarray> &matrix, Matrix<sizeA, sizeB, Tarray> const &add) {
    size_t indexA = 0;
    size_t indexB = 0;

    for (indexA = 0; indexA < sizeA; indexA++) {
        for (indexB = 0; indexB < sizeB; indexB++) {
            matrix.array[indexA][indexB] += add.array[indexA][indexB];
        }
    }

    return matrix;
}

// class MatrixBuilder {
// public:
//     static Matrix<3, 3, uint16_t> BuildOrientationMatrix(const OrientationData_t &data) {
//         Matrix<3, 3, uint16_t> result;
//         result[0][0] = data.acceleration.x;
//         result[0][1] = data.acceleration.y;
//         result[0][2] = data.acceleration.z;
//         result[1][0] = data.rotation.x;
//         result[1][1] = data.rotation.y;
//         result[1][2] = data.rotation.z;
//         result[2][0] = data.magnetometer.x;
//         result[2][1] = data.magnetometer.y;
//         result[2][2] = data.magnetometer.z;
//         return result;
//     }
// };

#endif // __CODE_GRAV_SYSTEM_TYPES_H__
