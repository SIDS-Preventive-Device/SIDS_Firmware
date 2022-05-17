#include "system/modules/calibration.h"
#include "system/kernel/core.h"
#include "system/modules/orientation.h"
#include "system/modules/logger.h"
#include "system/kernel/calls.h"
#include "system/kernel/nvstorage.h"
#include "system/utils/events.h"
#include "system/utils/linkedlist.h"

const char* KEY_CALIBRATION_GIROSCOPE = "cal_giro";
const char* KEY_CALIBRATION_ACCELEROMETER = "cal_acc";
const char* KEY_CALIBRATION_MAGNETOMETER = "cal_mag";

AverageOffsetsCalibrations<Vector3D_t> GiroscopeCalibration (KEY_CALIBRATION_GIROSCOPE);
AverageOffsetsCalibrations<Vector3D_t> AccelerometerCalibration (KEY_CALIBRATION_ACCELEROMETER);
AverageOffsetsCalibrations<Vector3D_t> MagnetometerCalibration (KEY_CALIBRATION_MAGNETOMETER);
