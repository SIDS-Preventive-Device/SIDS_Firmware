#include "system/modules/orientation.h"
#include "system/drivers/mpu9250.h"
#include "system/kernel/core.h"

Matrix<3, 1, int16_t>  fromVector3D_t(Vector3D_t origin) {
    Matrix<3, 1, int16_t>  vec;
    vec[0][0] = origin.x;
    vec[1][0] = origin.y;
    vec[2][0] = origin.z;
    return vec;
}

class OrientationFilter {
    Matrix<3, 1, float> AccOffsets;
    Matrix<3, 1, float> GirOffsets;
    Matrix<3, 1, float> MagOffsets;

    Matrix<3, 3, float> AccCorrection;
    Matrix<3, 3, float> GirCorrection;
    Matrix<3, 3, float> MagCorrection;

    Matrix<3, 1, int16_t> AccMeasures;
    Matrix<3, 1, int16_t> GirMeasures;
    Matrix<3, 1, int16_t> MagMeasures;
};
