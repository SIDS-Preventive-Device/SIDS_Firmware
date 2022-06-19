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

class MahonyFilter2D
{
public:
    MahonyFilter2D() {}
    void MahonyQuaternionUpdate(float ax, float ay, float az, float gx, float gy, float gz, float dT)
    {
        float recipNorm;
        float vx, vy, vz;
        float ex, ey, ez; // error terms
        float qa, qb, qc;
        static float ix = 0.0, iy = 0.0, iz = 0.0; // integral feedback terms
        float tmp;

        // Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
        tmp = ax * ax + ay * ay + az * az;
        if (tmp > 0.0)
        {

            // Normalise accelerometer (assumed to measure the direction of gravity in body frame)
            recipNorm = 1.0 / sqrt(tmp);
            //ax *= recipNorm;
            //ay *= recipNorm;
           // az *= recipNorm;

            // Estimated direction of gravity in the body frame (factor of two divided out)
            vx = q[1] * q[3] - q[0] * q[2]; // to normalize these terms, multiply each by 2.0
            vy = q[0] * q[1] + q[2] * q[3];
            vz = q[0] * q[0] - 0.5f + q[3] * q[3];

            // Error is cross product between estimated and measured direction of gravity in body frame
            // (half the actual magnitude)
            ex = (ay * vz - az * vy);
            ey = (az * vx - ax * vz);
            ez = (ax * vy - ay * vx);

            // Compute and apply to gyro term the integral feedback, if enabled
            if (KI > 0.0f)
            {
                ix += KI * ex * dT; // integral error scaled by KI
                iy += KI * ey * dT;
                iz += KI * ez * dT;
                gx += ix; // apply integral feedback
                gy += iy;
                gz += iz;
            }

            // Apply proportional feedback to gyro term
            gx += KP * ex;
            gy += KP * ey;
            gz += KP * ez;
        }

        // Integrate rate of change of quaternion, q cross gyro term
        dT = 0.5 * dT;
        gx *= dT; // pre-multiply common factors
        gy *= dT;
        gz *= dT;
        qa = q[0];
        qb = q[1];
        qc = q[2];
        q[0] += (-qb * gx - qc * gy - q[3] * gz);
        q[1] += (qa * gx + qc * gz - q[3] * gy);
        q[2] += (qa * gy - qb * gz + q[3] * gx);
        q[3] += (qa * gz + qb * gy - qc * gx);

        // renormalise quaternion
        recipNorm = 1.0 / sqrt(q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
        q[0] = q[0] * recipNorm;
        q[1] = q[1] * recipNorm;
        q[2] = q[2] * recipNorm;
        q[3] = q[3] * recipNorm;
    }
    int getRollPitchYaw(float *navigationAngles)
    {
        //*Formulas sacadas de aquí: http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles, no hay que olvidar que en este tipo de representación,
        //*los angulos no son conmutativos, una rotación afecta a la otra, en este caso, un pitch >= +-90, puede afectar al yaw.

        navigationAngles[0] = atan2((this->q[0] * this->q[1] + this->q[2] * this->q[3]), 0.5f - (this->q[1] * this->q[1] + this->q[2] * this->q[2]));
        navigationAngles[1] = asin(2.0f * (this->q[0] * this->q[2] - this->q[1] * this->q[3]));
        navigationAngles[2] = atan2((this->q[1] * this->q[2] + this->q[0] * this->q[3]), 0.5f - (this->q[2] * this->q[2] + this->q[3] * this->q[3]));
        return 0;
    }
    int getQuaternion(float *quaternion)
    {
        quaternion[0] = this->q[0];
        quaternion[1] = this->q[1];
        quaternion[2] = this->q[2];
        quaternion[3] = this->q[3];
        return 0;
    }

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

extern EulerMatrix_t CalculateOrientation(OrientationData_t &orientationData, OrientationParams_t params);

extern uint8_t CalculatePositionRisk(EulerMatrix_t quaternions);

#endif // __CODE_GRAV_SYSTEM_MODULES_ORIENTATION_H__
