#include "system/modules/orientation.h"
#include "system/drivers/mpu9250.h"
#include "system/kernel/core.h"
#include "system/kernel/ble.h"

static MahonyFilter mahony;
  
//Offsets obtenidos para el acelerometro y su matriz de corrección, donde solo hacemos uso de los valores en la diagonal.
float A_B[3]  {  150.42,   94.82, -725.65};

float A_Ainv[3][3]  {
  {  0.06220, -0.00141, -0.00018},
  { -0.00141,  0.06138, -0.00048},
  { -0.00018, -0.00048,  0.05994}
};

// Offsets obtenidos para el magnetometro y su matriz de corrección, donde solo hacemos uso de los valores en la diagonal.
float M_B[3]  {    6.36,   25.27,  -69.48};

float M_Ainv[3][3] {
  {  1.75453, -0.09176,  0.03732},
  { -0.09176,  1.85748,  0.09509},
  {  0.03732,  0.09509,  1.94828}
};

QuaternionMatrix_t CalculateOrientation(OrientationData_t& orientationData, OrientationParams_t params)
{
    QuaternionMatrix_t result;

    //
    // Calibrate giroscope measure
    //
    Matrix<3, 1, float> vGiro = (orientationData.rotation.toMatrix() - params.giroscopeOffsets) * params.giroScale;
    Matrix<3, 1, float> vAcc = orientationData.acceleration.toMatrix() - params.accelerometerOffsets;
    Matrix<3, 1, float> vMag = orientationData.magnetometer.toMatrix() - params.accelerometerOffsets;


    //Los ejes estan invertidos por las siguientes razones:
    // 1.- Por conveniencia y la forma de la placa.
    // 2.- Los ejes x & y del magnetometro estan invertidos con respecto al acelerometro y magnetometro
    mahony.MahonyQuaternionUpdate(vAcc[1][0], vAcc[0][0], vAcc[2][0], vGiro[1][0], vGiro[0][0], vGiro[2][0],
                                    vMag[0][0], vMag[1][0], -vMag[2][0], params.measureInterval);

    mahony.getQuaternion(result.getInternalArrayPtr());

    char buffer[32];
    sprintf(buffer, "%f %f %f", result[0][0], result[1][0], result[2][0], result[3][0]);
    OsKernel::SetBLECharacteristicValue(BLE_CHT_POSITION, buffer);

    return result;
}

uint8_t CalculatePositionRisk (QuaternionMatrix_t quaternions) {
    return 0;
}

MahonyFilter::MahonyFilter() {}

MahonyFilter::MahonyFilter(float KP, float KI)
{
    this->KP = KP;
    this->KI = KI;
}

void MahonyFilter::MahonyQuaternionUpdate(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz, float dT)
{
    static float eInt[3] = {0.0, 0.0, 0.0};

    float q1 = this->q[0];
    float q2 = this->q[1];
    float q3 = this->q[2];
    float q4 = this->q[3];

    float norm;
    float hx, hy, bx, bz;
    float vx, vy, vz, wx, wy, wz;
    float ex, ey, ez;
    float qa, qb, qc;

    float q1q1 = q1 * q1;
    float q1q2 = q1 * q2;
    float q1q3 = q1 * q3;
    float q1q4 = q1 * q4;
    float q2q2 = q2 * q2;
    float q2q3 = q2 * q3;
    float q2q4 = q2 * q4;
    float q3q3 = q3 * q3;
    float q3q4 = q3 * q4;
    float q4q4 = q4 * q4;

    // Reference direction of Earth's magnetic field
    hx = 2.0f * mx * (0.5f - q3q3 - q4q4) + 2.0f * my * (q2q3 - q1q4) + 2.0f * mz * (q2q4 + q1q3);
    hy = 2.0f * mx * (q2q3 + q1q4) + 2.0f * my * (0.5f - q2q2 - q4q4) + 2.0f * mz * (q3q4 - q1q2);
    bx = sqrt((hx * hx) + (hy * hy));
    bz = 2.0f * mx * (q2q4 - q1q3) + 2.0f * my * (q3q4 + q1q2) + 2.0f * mz * (0.5f - q2q2 - q3q3);

    // Estimated direction of gravity and magnetic field
    vx = 2.0f * (q2q4 - q1q3);
    vy = 2.0f * (q1q2 + q3q4);
    vz = q1q1 - q2q2 - q3q3 + q4q4;
    wx = 2.0f * bx * (0.5f - q3q3 - q4q4) + 2.0f * bz * (q2q4 - q1q3);
    wy = 2.0f * bx * (q2q3 - q1q4) + 2.0f * bz * (q1q2 + q3q4);
    wz = 2.0f * bx * (q1q3 + q2q4) + 2.0f * bz * (0.5f - q2q2 - q3q3);

    // Error is cross product between estimated direction and measured direction of the reference vectors
    ex = (ay * vz - az * vy) + (my * wz - mz * wy);
    ey = (az * vx - ax * vz) + (mz * wx - mx * wz);
    ez = (ax * vy - ay * vx) + (mx * wy - my * wx);
    if (this->KI > 0.0f)
    {
        eInt[0] += ex; // accumulate integral error
        eInt[1] += ey;
        eInt[2] += ez;
        // Apply I feedback
        gx += this->KI * eInt[0];
        gy += this->KI * eInt[1];
        gz += this->KI * eInt[2];
    }

    // Apply P feedback
    gx = gx + this->KP * ex;
    gy = gy + this->KP * ey;
    gz = gz + this->KP * ez;

    gx = gx * (0.5 * dT); // pre-multiply common factors
    gy = gy * (0.5 * dT);
    gz = gz * (0.5 * dT);

    qa = q1;
    qb = q2;
    qc = q3;

    q1 += (-qb * gx - qc * gy - q4 * gz);
    q2 += (qa * gx + qc * gz - q4 * gy);
    q3 += (qa * gy - qb * gz + q4 * gx);
    q4 += (qa * gz + qb * gy - qc * gx);

    // Normalise quaternion
    norm = 1.0f / (quaternionNormalize(q1, q2, q3, q4));
    this->q[0] = q1 * norm;
    this->q[1] = q2 * norm;
    this->q[2] = q3 * norm;
    this->q[3] = q4 * norm;
}

int MahonyFilter::getRollPitchYaw(float *navigationAngles)
{
    //*Formulas sacadas de aquí: http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles, no hay que olvidar que en este tipo de representación,
    //*los angulos no son conmutativos, una rotación afecta a la otra, en este caso, un pitch >= +-90, puede afectar al yaw.

    navigationAngles[0] = -atan2((this->q[0] * this->q[1] + this->q[2] * this->q[3]), 0.5f - (this->q[1] * this->q[1] + this->q[2] * this->q[2]));
    navigationAngles[1] = asin(2.0f * (this->q[0] * this->q[2] - this->q[1] * this->q[3]));
    navigationAngles[2] = atan2((this->q[1] * this->q[2] + this->q[0] * this->q[3]), 0.5f - (this->q[2] * this->q[2] + this->q[3] * this->q[3]));
    return 0;
}

int MahonyFilter::getQuaternion(float *quaternion)
{
    quaternion[0] = this->q[0];
    quaternion[1] = this->q[1];
    quaternion[2] = this->q[2];
    quaternion[3] = this->q[3];
    return 0;
}

float MahonyFilter::quaternionNormalize(float qw, float qx, float qy, float qz)
{
    return sqrt((qw * qw) + (qx * qx) + (qy * qy) + (qz * qz));
}

void MahonyFilter::setKP(float KP)
{
    this->KP = KP;
}

void MahonyFilter::setKI(float KI)
{
    this->KI = KI;
}

float MahonyFilter::getKP()
{
    return this->KP;
}

float MahonyFilter::getKI()
{
    return this->KI;
}