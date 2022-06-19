#include "system/drivers/led.h"

#define __ALERT_TIME_BY_DEFAULT 5000

Led::Led (uint8_t pin)
    : pin (pin) {
    this->timerLedOffConfig = {
        .callback = [](void* pPinNumber) -> void {
            digitalWrite(*((uint8_t*)pPinNumber), LOW);
        },
        .arg = &pin,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "LedAlrtDown"
    };

    esp_timer_create (&this->timerLedOffConfig, &this->timerLedOff_h);
}

void Led::Configure ()
{
    logger << LOG_INFO << F("Configuring Led on pin ") << pin << EndLine;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void Led::Alert(uint32_t milliseconds) const
{
    if (milliseconds == 0) {
        milliseconds = __ALERT_TIME_BY_DEFAULT;
    }
    logger << LOG_INFO << F("Alert throwed for ") << milliseconds << F(" ms") << EndLine;
    digitalWrite(pin, HIGH);
    esp_timer_start_once (timerLedOff_h, milliseconds * 1000UL);
}
