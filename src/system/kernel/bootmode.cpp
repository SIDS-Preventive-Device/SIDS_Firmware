#include "system/kernel/bootmode.h"
#include "system/utils/array_size.h"

const __FlashStringHelper *OsKernel::BootModeStrMap[] = {
    F("NORMAL"),
    F("CALIBRATION")
};

const __FlashStringHelper *OsKernel::GetBootModeStr(SystemBootMode_t mode)
{
    if (mode >= ARRAY_SIZE(OsKernel::BootModeStrMap)) {
        return F("UNKNOWN");
    }
    return OsKernel::BootModeStrMap[(uint8_t)mode];
}
