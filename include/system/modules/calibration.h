#if !defined(__CODE_GRAV_SYSTEM_MODULES_CALIBRATION_H__)
#define __CODE_GRAV_SYSTEM_MODULES_CALIBRATION_H__

#include <stdint.h>
#include "system/utils/types.h"

typedef struct {
    uint16_t measureIntervalMs;
    uint16_t measuringTimeS;
} CalibrationConfig_t;

extern Vector3D_t CalibrateGiroscope(CalibrationConfig_t config);
extern Vector3D_t CalibrateGiroscope();
extern Vector3D_t GetStoredGiroscopeCalibrations();
extern bool       StoreGiroscopeCalibrationResults(Vector3D_t calibratedOffsets);

#endif // __CODE_GRAV_SYSTEM_MODULES_CALIBRATION_H__
