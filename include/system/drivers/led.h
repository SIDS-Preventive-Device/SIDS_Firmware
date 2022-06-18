#if !defined(__CODE_GRAV_SYSTEM_DRIVERS_LED_H__)
#define __CODE_GRAV_SYSTEM_DRIVERS_LED_H__

#include <system/utils/types.h>
#include "system/kernel/core.h"

class Led : public IRiskAlert
{
    esp_timer_handle_t timerLedOff_h;
    esp_timer_create_args_t timerLedOffConfig;
    uint8_t pin;
public:
    Led(uint8_t pin = LED_BUILTIN)
        : pin(pin)
    { }

    void Configure ();
    void Alert(uint32_t milliseconds = 0) const;

    ~Led();
};

#endif // __CODE_GRAV_SYSTEM_DRIVERS_LED_H__
