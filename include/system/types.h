#if !defined(__CODE_GRAV_SYSTEM_TYPES_H__)
#define __CODE_GRAV_SYSTEM_TYPES_H__

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "system/utils/array_size.h"
#if !defined(ARDUINO) && defined(UNITY)
#include <iostream>
#endif
#if defined(ARDUINO)
#include <Print.h>
#include <Printable.h>
#endif

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

template <size_t sizeA, size_t sizeB, typename Tarray>
class Matrix
#if defined(ARDUINO)
    : public Printable
#endif
{
#if defined(UNITY)
public:
#endif
    Tarray array[sizeA][sizeB];

public:
    Matrix() {
        memset(array, 0x00, sizeof(Tarray) * sizeA * sizeB);
    }

    Matrix(Tarray inputArray[sizeA][sizeB]) {
        memset(array, 0x00, sizeof(Tarray) * sizeA * sizeB);
        size_t indexA = 0;
        size_t indexB = 0;

        for (indexA = 0; indexA < sizeA; indexA++) {
            for (indexB = 0; indexB < sizeB; indexB++) {
                array[indexA][indexB] = inputArray[indexA][indexB];
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

    size_t cols() const { return sizeB; }
    size_t rows() const { return sizeA; }
    size_t elements() const { return sizeA * sizeB; }

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

    template <size_t sizeC, typename Tarray2>
    Matrix<sizeA, sizeC, Tarray> operator *(Matrix<sizeB, sizeC, Tarray2> &matrix2) {
        Matrix<sizeA, sizeC, Tarray> result;
        size_t indexA = 0;
        size_t indexB = 0;
        size_t indexC = 0;


        for (indexA = 0; indexA < sizeA; indexA++) {
            for (indexC = 0; indexC < sizeC; indexC++) {
                result[indexA][indexC] = 0;
                for (indexB = 0; indexB < sizeB; indexB++) {
                    result[indexA][indexC] += array[indexA][indexB] * matrix2.array[indexB][indexC];
                }
            }
        }
        
        return result;
    }

    /**
     * @brief Transpose matrix
     * 
     * @param matrix2 
     * @return Matrix<sizeB, sizeB, Tarray> 
     */
    Matrix<sizeB, sizeA, Tarray> operator ~() {
        Matrix<sizeB, sizeA, Tarray> transpose;
        size_t indexA = 0;
        size_t indexB = 0;

        for (indexA = 0; indexA < sizeA; indexA++) {
            for (indexB = 0; indexB < sizeB; indexB++) {
                transpose[indexB][indexA] = array[indexA][indexB];
            }
        }

        return transpose;
    }

#if defined(ARDUINO)
    size_t printTo(Print& p) const {
        p.println();
        size_t indexA = 0;
        size_t indexB = 0;
        size_t c = 0;
        for (indexA = 0; indexA < sizeA; indexA++) {
            for (indexB = 0; indexB < sizeB; indexB++) {
                c += p.print(String(array[indexA][indexB]));
                c += p.print(F(" "));
            }
            p.println();
        }
        return c;
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
