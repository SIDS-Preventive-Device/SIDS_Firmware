#include "system/calltable.h"
#include "system/bootmode.h"
#include "system/utils/array_size.h"

const __FlashStringHelper *BootModeStrMap[] = {
    F("Normal"),
    F("Calibration")
};

extern const __FlashStringHelper *GetBootModeStr(SystemBootMode_t mode) {
    if (mode >= ARRAY_SIZE(BootModeStrMap)) {
        return F("UNKNOWN");
    }
    return BootModeStrMap[(uint8_t)mode];
}

BootModesCallTableEntry_t BootModesCallTable[] = {
//-----------------------------------------------------------------------------
//    Boot Mode               Entry Point
//-----------------------------------------------------------------------------
    { BOOT_NORMAL,            KERNEL_BOOT_THREAD_NAME(BOOT_NORMAL)          },
    { BOOT_CALIBRATION,       KERNEL_BOOT_THREAD_NAME(BOOT_CALIBRATION)     }
};

const size_t BootModesCallTableSize = ARRAY_SIZE(BootModesCallTable);

BootModeEntry_f GetEntryPointForBootMode(SystemBootMode_t mode)
{
    size_t index;
    for (index = 0; index < BootModesCallTableSize; index++)
    {
        if (BootModesCallTable[index].mode == mode) {
            return BootModesCallTable[index].entryPoint;
        }
    }

    return NULL;
}