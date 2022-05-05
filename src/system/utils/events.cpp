#include "system/utils/events.h"

#include <Arduino.h>

bool wait(WaitCallback_t pollFunction, void *pContext, uint32_t timeout_ms, Print *cout, char waitChar)
{
    uint32_t print_ms_mod = timeout_ms / 10UL;
    uint32_t elapsed = 0;
    uint32_t last_print_ms = 0;
    uint32_t st_ticks = millis();
    
    while (!pollFunction(pContext)) {
        elapsed = millis() - st_ticks;

        if (elapsed >= timeout_ms) {
            if (cout) {
                cout->println();
            }
            return false;
        }

        if (cout && ((elapsed - last_print_ms) > print_ms_mod)) {
            last_print_ms = elapsed;
            cout->print(waitChar);
        }
    }

    if (cout) {
        cout->println();
    }

    return true;
}
