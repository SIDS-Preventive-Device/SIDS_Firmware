#include "system/drivers/buzzer.h"
#include "system/kernel/core.h"

#define __ALERT_TIME_BY_DEFAULT 5000

Buzzer::Buzzer (uint8_t pin)
    : pin(pin) {
    this->updateSensorTimerConfig = {
        .callback = [](void* pPinNumber) -> void {
            digitalWrite(*((uint8_t*)pPinNumber), LOW);
        },
        .arg = &pin,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "AlrtDown"
    };

    esp_timer_create (&this->updateSensorTimerConfig, &this->updateSensorsTimer_h);
}

void Buzzer::Configure ()
{
    pinMode(pin, OUTPUT);
    digitalWrite (pin, LOW);
}

void Buzzer::Alert(uint32_t milliseconds) const
{
    if (milliseconds == 0) {
        milliseconds = __ALERT_TIME_BY_DEFAULT;
    }
    //
    // Start the alert and and shutdown the buzzer in async process.
    //
    digitalWrite (pin, HIGH);
    esp_timer_start_once (updateSensorsTimer_h, milliseconds * 1000UL);
}
