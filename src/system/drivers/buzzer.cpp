#include "system/drivers/buzzer.h"
#include "system/kernel/core.h"
#include "system/modules/logger.h"

#define __ALERT_TIME_BY_DEFAULT 5000

Buzzer::Buzzer (uint8_t pin)
    : pin(pin) {
    this->updateSensorTimerConfig = {
        .callback = [](void* pPinNumber) -> void {
            ledcWrite(0, 0);
        },
        .arg = &pin,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "AlrtDown"
    };

    esp_timer_create (&this->updateSensorTimerConfig, &this->updateSensorsTimer_h);
}

void Buzzer::Configure ()
{
    logger << LOG_INFO << F("Configuring Buzzer on pin ") << pin << EndLine;
    ledcAttachPin(pin, 0);
    ledcSetup(0, 1100, 8);
}

void Buzzer::Alert(uint32_t milliseconds) const
{
    if (milliseconds == 0) {
        milliseconds = __ALERT_TIME_BY_DEFAULT;
    }
    logger << LOG_INFO << F("Alert throwed for ") << milliseconds << F(" ms") << EndLine;
    ledcWrite(0, 0x7F);
    esp_timer_start_once (updateSensorsTimer_h, milliseconds * 1000UL);
}
