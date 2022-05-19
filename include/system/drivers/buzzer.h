#if !defined(__CODE_GRAV_SYSTEM_DRIVERS_BUZZER_H__)
#define __CODE_GRAV_SYSTEM_DRIVERS_BUZZER_H__

#include <system/utils/isensor.h>
#include <system/utils/types.h>
#include "system/kernel/core.h"

#define DEFAULT_BUZZER_PIN 10

class Buzzer : public IRiskAlert
{
    uint8_t pin;
    esp_timer_handle_t updateSensorsTimer_h;
    esp_timer_create_args_t updateSensorTimerConfig;
public:
    Buzzer (uint8_t pin = DEFAULT_BUZZER_PIN);

    void Configure ();
    void Alert (uint32_t milliseconds = 0) const;
};

#endif // __CODE_GRAV_SYSTEM_DRIVERS_BUZZER_H__
