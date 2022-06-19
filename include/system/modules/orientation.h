#if !defined(__CODE_GRAV_SYSTEM_MODULES_ORIENTATION_H__)
#define __CODE_GRAV_SYSTEM_MODULES_ORIENTATION_H__

#include "system/utils/matrix.h"
#include "system/utils/types.h"

typedef Matrix<4, 1, float> QuaternionMatrix_t;
typedef Matrix<3, 1, float> EulerMatrix_t;

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

    QuaternionMatrix_t getQuaternionMatrix() const;
    EulerMatrix_t getRollPitchYawMatrix();

private:
    float KP = KP_DEFAULT;
    float KI = KI_DEFAULT;
    float q[4] = {1.000, 0.000, 0.000, 0.000};
    float quaternionNormalize(float qw, float qx, float qy, float qz);
};

typedef struct
{
    float giroScale;
    uint32_t measureInterval;
} OrientationParams_t;

extern EulerMatrix_t CalculateOrientation (OrientationData_t& orientationData, OrientationParams_t params);

extern uint8_t CalculatePositionRisk (EulerMatrix_t quaternions);

#endif // __CODE_GRAV_SYSTEM_MODULES_ORIENTATION_H__
