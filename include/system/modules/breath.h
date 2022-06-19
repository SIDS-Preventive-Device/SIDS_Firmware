#if !defined(__CODE_GRAV_SYSTEM_MODULES_BREAH_H__)
#define __CODE_GRAV_SYSTEM_MODULES_BREAH_H__

#include "system/utils/types.h"

typedef enum {
    BREATH_DETECTED,
    BREATH_NOT_DETECTED,
    BREATH_INITIALIZING,
    BREATH_DELIM
} BreathStatus_e;

extern const __FlashStringHelper* GetBreathStatusStr (BreathStatus_e st);

extern BreathStatus_e isBreathMovementDetected (OrientationData_t &orientation);

#endif // __CODE_GRAV_SYSTEM_MODULES_BREAH_H__
