#include "system/modules/orientation.h"

#include "system/drivers/mpu9250.h"
#include "system/kernel/ble.h"
#include "system/kernel/core.h"

static MahonyFilter mahony;

#define gscale (250.0 / 32768.0) * (PI / 180.0) // gyro default 250 LSB per d/s -> rad/s

// Descomentar la siguiente linea para imprimir en formato de quaterniones en lugar de angulos navales.
//#define QUATERNIONS 1

// Offsets obtenidos para el acelerometro y su matriz de corrección, donde solo hacemos uso de los valores en la diagonal.
float A_B[3]{86.16, -74.97, -450.39};

float A_Ainv[3][3]{
    {0.06144, 0.00177, -0.00034},
    {0.00177, 0.06036, -0.00081},
    {-0.00034, -0.00081, 0.05985}};

// Offsets obtenidos para el magnetometro y su matriz de corrección, donde solo hacemos uso de los valores en la diagonal.
float M_B[3]{29.61, 37.55, -312.00};

float M_Ainv[3][3]{
    {0.48370, -0.00397, -0.00072},
    {-0.00397, 0.48879, -0.00555},
    {-0.00072, -0.00555, 0.47706}};

float G_off[3] = {-145.8, -145.8, -145.8}; // raw offsets, determined for gyro at rest

int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t mx, my, mz;
float Axyz[3];
float Gxyz[3];
float Mxyz[3];

unsigned long now = 0, last = 0;
float dT = 0;
unsigned long now_ms, last_ms = 0;
unsigned long print_ms = 70; // Cada cuanto queremos imprimir las mediciones.

bool initial = true;
float initialYaw;
int count = 0;

float vectorDot(float a[3], float b[3]);
void vectorNormalize(float a[3]);

EulerMatrix_t CalculateOrientation(OrientationData_t &orientationData, OrientationParams_t params)
{
    char buffer[64];
    EulerMatrix_t result;
    float temp[3];
    float vGiro[3];
    float vAcc[3];
    float vMag[3];
    float rotation[3];
    float dT = 0;
    float rollPitchYaw[3];
    float q[4];

    int16_t ax = orientationData.acceleration.x;
    int16_t ay = orientationData.acceleration.y;
    int16_t az = orientationData.acceleration.z;
    int16_t gx = orientationData.rotation.x;
    int16_t gy = orientationData.rotation.y;
    int16_t gz = orientationData.rotation.z;
    int16_t mx = orientationData.magnetometer.x;
    int16_t my = orientationData.magnetometer.y;
    int16_t mz = orientationData.magnetometer.z;

    // printf("%d %d %d\n%d %d %d\n%d %d %d\n", gx, gy, gz, ax, ay, az, mx, my, mz);

    Gxyz[0] = ((float)gx - G_off[0]) * gscale;
    Gxyz[1] = ((float)gy - G_off[1]) * gscale;
    Gxyz[2] = ((float)gz - G_off[2]) * gscale;

    Axyz[0] = (float)ax;
    Axyz[1] = (float)ay;
    Axyz[2] = (float)az;
    // Se aplican los offsets y factores de escala obtenidos de magneto
    for (int i = 0; i < 3; i++)
        temp[i] = (Axyz[i] - A_B[i]);
    Axyz[0] = A_Ainv[0][0] * temp[0] + A_Ainv[0][1] * temp[1] + A_Ainv[0][2] * temp[2];
    Axyz[1] = A_Ainv[1][0] * temp[0] + A_Ainv[1][1] * temp[1] + A_Ainv[1][2] * temp[2];
    Axyz[2] = A_Ainv[2][0] * temp[0] + A_Ainv[2][1] * temp[1] + A_Ainv[2][2] * temp[2];
    vectorNormalize(Axyz);

    Mxyz[0] = (float)mx;
    Mxyz[1] = (float)my;
    Mxyz[2] = (float)mz;

    // Se aplican los offsets y factores de escala obtenidos de magneto
    for (int i = 0; i < 3; i++)
        temp[i] = (Mxyz[i] - M_B[i]);
    Mxyz[0] = M_Ainv[0][0] * temp[0] + M_Ainv[0][1] * temp[1] + M_Ainv[0][2] * temp[2];
    Mxyz[1] = M_Ainv[1][0] * temp[0] + M_Ainv[1][1] * temp[1] + M_Ainv[1][2] * temp[2];
    Mxyz[2] = M_Ainv[2][0] * temp[0] + M_Ainv[2][1] * temp[1] + M_Ainv[2][2] * temp[2];
    vectorNormalize(Mxyz);

    now = micros();
    dT = (now - last) * 1.0e-6; // Tiempo que ha pasado desde la última medición
    last = now;

    // Los ejes estan invertidos por las siguientes razones:
    //* 1.- Por conveniencia y la forma de la placa.
    //* 2.- Los ejes x & y del magnetometro estan invertidos con respecto al acelerometro y magnetometro
    mahony.MahonyQuaternionUpdate(Axyz[1], Axyz[0], Axyz[2], Gxyz[1], Gxyz[0], Gxyz[2],
                                  Mxyz[0], Mxyz[1], -Mxyz[2], dT);
    // mahony.MahonyQuaternionUpdate(Axyz[1], Axyz[0], Axyz[2], Gxyz[1], Gxyz[0], Gxyz[2], dT);

    //*Obtenemos los valores de los angulos
    mahony.getRollPitchYaw(rollPitchYaw);

    //*Conversión de radianes a grados

    rollPitchYaw[2] *= 180.0 / PI;
    rollPitchYaw[1] *= 180.0 / PI;
    rollPitchYaw[0] *= 180.0 / PI;

    rollPitchYaw[2] = -rollPitchYaw[2] - 5.8;
    if (rollPitchYaw[2] < 0)
        rollPitchYaw[2] += 360.0;
    if (rollPitchYaw[2] > 360.0)
        rollPitchYaw[2] -= 360.0;

    memset(buffer, 0, 64);
    sprintf(buffer, "%.2f %.2f %.2f", rollPitchYaw[0], rollPitchYaw[1], rollPitchYaw[2]);
    OsKernel::SetBLECharacteristicValue(BLE_CHT_POSITION, buffer);
    logger << "Orientation: " << buffer << "\n";

    return result;
}

uint8_t CalculatePositionRisk(EulerMatrix_t quaternions)
{
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

    navigationAngles[0] = atan2((this->q[0] * this->q[1] + this->q[2] * this->q[3]), 0.5f - (this->q[1] * this->q[1] + this->q[2] * this->q[2]));
    navigationAngles[1] = asin(2.0f * (this->q[0] * this->q[2] - this->q[1] * this->q[3]));
    navigationAngles[2] = atan2((this->q[1] * this->q[2] + this->q[0] * this->q[3]), 0.5f - (this->q[2] * this->q[2] + this->q[3] * this->q[3]));
    return 0;
}

EulerMatrix_t MahonyFilter::getRollPitchYawMatrix()
{
    EulerMatrix_t output;
    this->getRollPitchYaw(output.getInternalArrayPtr());
    return output;
}

int MahonyFilter::getQuaternion(float *quaternion)
{
    quaternion[0] = this->q[0];
    quaternion[1] = this->q[1];
    quaternion[2] = this->q[2];
    quaternion[3] = this->q[3];
    return 0;
}

QuaternionMatrix_t MahonyFilter::getQuaternionMatrix() const
{
    QuaternionMatrix_t output;
    output[0][0] = this->q[0];
    output[1][0] = this->q[1];
    output[2][0] = this->q[2];
    output[3][0] = this->q[3];
    return output;
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
float vectorDot(float a[3], float b[3])
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

void vectorNormalize(float a[3])
{
    float magnitude = sqrt(vectorDot(a, a));
    a[0] /= magnitude;
    a[1] /= magnitude;
    a[2] /= magnitude;
}
