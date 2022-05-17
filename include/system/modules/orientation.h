#if !defined(__CODE_GRAV_SYSTEM_MODULES_ORIENTATION_H__)
#define __CODE_GRAV_SYSTEM_MODULES_ORIENTATION_H__

#include "system/utils/matrix.h"
#include "system/utils/types.h"

#define KP_DEFAULT 32.500
#define KI_DEFAULT 0.000

class MahonyFilter
{
public:
    MahonyFilter();
    MahonyFilter(float KP, float KI);
    void MahonyQuaternionUpdate(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz, float dT);
    int getRollPitchYaw(float *navigationAngles);
    int getQuaternion(float *quaternion);
    void setKP(float KP);
    void setKI(float KI);
    float getKP();
    float getKI();

private:
    float KP = KP_DEFAULT;
    float KI = KI_DEFAULT;
    float q[4] = {1.000, 0.000, 0.000, 0.000};
    float quaternionNormalize(float qw, float qx, float qy, float qz);
};

typedef struct
{
    Matrix<3, 1, int16_t> giroscopeOffsets;
    float giroScale;

    Matrix<3, 1, int16_t> accelerometerOffsets;
    Matrix<3, 3, float> accelerometerCorrection;
    Matrix<3, 1, int16_t> magnetometerOffsets;
    Matrix<3, 3, float> magnetometerCorrection;
} OrientationParams_t;

extern Matrix<3, 1, float> CalculateOrientation(OrientationParams_t params);

#endif // __CODE_GRAV_SYSTEM_MODULES_ORIENTATION_H__
