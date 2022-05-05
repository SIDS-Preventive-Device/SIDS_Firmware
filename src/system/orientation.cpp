#include "system/modules/orientation.h"
#include "system/drivers/mpu9250.h"

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

    Matrix<3, 1, float> AccMeasures;
    Matrix<3, 1, float> GirMeasures;
    Matrix<3, 1, float> MagMeasures;
public:
    OrientationFilter (
        OrientationData_t* ptr,
        TaskHandle_t updateTask_h
    );

    bool Calibrate();

private:
    void getMeasuresInRest ();
};
