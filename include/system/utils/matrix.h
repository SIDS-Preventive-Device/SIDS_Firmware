#if !defined(__CODE_GRAV_SYSTEM_UTILS_MATRIX_H__)
#define __CODE_GRAV_SYSTEM_UTILS_MATRIX_H__

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "system/utils/array_size.h"
#if !defined(ARDUINO) && defined(UNITY)
#include <iostream>
#endif
#if defined(ARDUINO)
#include <WString.h>
#include <Print.h>
#include <Printable.h>
#endif

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

    template <typename Tarray2>
    Matrix(Matrix<sizeA, sizeB, Tarray2> const &input) {
        memset(array, 0x00, sizeof(Tarray) * sizeA * sizeB);
        size_t indexA = 0;
        size_t indexB = 0;

        for (indexA = 0; indexA < sizeA; indexA++) {
            for (indexB = 0; indexB < sizeB; indexB++) {
                array[indexA][indexB] = input[indexA][indexB];
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

    Tarray magnitude() {
        Tarray result;
        size_t indexA = 0;
        size_t indexB = 0;

        for (indexA = 0; indexA < sizeA; indexA++) {
            for (indexB = 0; indexB < sizeB; indexB++) {
                result += array[indexA][indexB] * array[indexA][indexB];
            }
        }

        return result;
    }

    void normalize() {
        Tarray mag = (Tarray)sqrt(this->magnitude());
        size_t indexA = 0;
        size_t indexB = 0;

        for (indexA = 0; indexA < sizeA; indexA++) {
            for (indexB = 0; indexB < sizeB; indexB++) {
                array[indexA][indexB] /= mag;
            }
        }
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

    Matrix<sizeA, sizeB, Tarray> operator -(Matrix const &matrix) {
        Matrix<sizeA, sizeB, Tarray> result;
        size_t indexA = 0;
        size_t indexB = 0;

        for (indexA = 0; indexA < sizeA; indexA++) {
            for (indexB = 0; indexB < sizeB; indexB++) {
                result[indexA][indexB] = matrix[indexA][indexB] - array[indexA][indexB];
            }
        }

        return result;
    }

    template <typename Tconst>
    Matrix<sizeA, sizeB, Tarray> operator +(Tconst const &constant) {
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

    template <typename Tconst>
    Matrix<sizeA, sizeB, Tarray> operator *(Tconst const &constant) {
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
                    result[indexA][indexC] += array[indexA][indexB] * matrix2[indexB][indexC];
                }
            }
        }
        
        return result;
    }

    Tarray operator ^(Matrix<sizeA, sizeB, Tarray> &matrix2) {
        Tarray result;
        size_t indexA = 0;
        size_t indexB = 0;

        for (indexA = 0; indexA < sizeA; indexA++) {
            for (indexB = 0; indexB < sizeB; indexB++) {
                result += array[indexA][indexB] * matrix2[indexA][indexB];
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

    String toString() {
        String str;
        size_t indexA = 0;
        size_t indexB = 0;
        for (indexA = 0; indexA < sizeA; indexA++) {
            for (indexB = 0; indexB < sizeB; indexB++) {
                str += String(array[indexA][indexB]) + " ";
            }
        }
        return str;
    }

    size_t printTo(Print& p) const {
        size_t indexA = 0;
        size_t indexB = 0;
        size_t c = 0;
        for (indexA = 0; indexA < sizeA; indexA++) {
            for (indexB = 0; indexB < sizeB; indexB++) {
                c += p.print(String(array[indexA][indexB]));
                c += p.print(F(" "));
            }
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
Tarray operator ^=(Matrix<sizeA, sizeB, Tarray> &matrix, Matrix<sizeA, sizeB, Tarray> &matrix2) {
    Tarray result;
    size_t indexA = 0;
    size_t indexB = 0;

    for (indexA = 0; indexA < sizeA; indexA++) {
        for (indexB = 0; indexB < sizeB; indexB++) {
            result += matrix[indexA][indexB] * matrix2[indexA][indexB];
        }
    }

    return result;
}

template <size_t sizeA, size_t sizeB, typename Tarray, typename Tconst>
Matrix<sizeA, sizeB, Tarray> operator *=(Matrix<sizeA, sizeB, Tarray> &matrix, Tconst const &constant) {
    size_t indexA = 0;
    size_t indexB = 0;

    for (indexA = 0; indexA < sizeA; indexA++) {
        for (indexB = 0; indexB < sizeB; indexB++) {
            matrix[indexA][indexB] *= constant;
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
            matrix[indexA][indexB] += add[indexA][indexB];
        }
    }

    return matrix;
}

template <size_t sizeA, size_t sizeB, typename Tarray>
Matrix<sizeA, sizeB, Tarray> operator +=(Matrix<sizeA, sizeB, Tarray> &matrix, Tarray add) {
    size_t indexA = 0;
    size_t indexB = 0;

    for (indexA = 0; indexA < sizeA; indexA++) {
        for (indexB = 0; indexB < sizeB; indexB++) {
            matrix[indexA][indexB] += add;
        }
    }

    return matrix;
}

template <size_t sizeA, size_t sizeB, typename Tarray>
Matrix<sizeA, sizeB, Tarray> operator -=(Matrix<sizeA, sizeB, Tarray> &matrix, Matrix<sizeA, sizeB, Tarray> const &add) {
    size_t indexA = 0;
    size_t indexB = 0;

    for (indexA = 0; indexA < sizeA; indexA++) {
        for (indexB = 0; indexB < sizeB; indexB++) {
            matrix[indexA][indexB] -= add[indexA][indexB];
        }
    }

    return matrix;
}

template <size_t sizeA, size_t sizeB, typename Tarray>
Matrix<sizeA, sizeB, Tarray> operator -=(Matrix<sizeA, sizeB, Tarray> &matrix, Tarray const &add) {
    size_t indexA = 0;
    size_t indexB = 0;

    for (indexA = 0; indexA < sizeA; indexA++) {
        for (indexB = 0; indexB < sizeB; indexB++) {
            matrix[indexA][indexB] -= add;
        }
    }

    return matrix;
}


#endif // __CODE_GRAV_SYSTEM_UTILS_MATRIX_H__
