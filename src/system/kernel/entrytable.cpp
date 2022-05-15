#include "system/kernel/entrytable.h"
#include "system/utils/array_size.h"
#include "system/kernel/nvstorage.h"
#include "kernel_internal.h"

BootModesCallTableEntry_t OsKernel::BootModesCallTable[] = {
//-----------------------------------------------------------------------------
//    Boot Mode               Entry Point
//-----------------------------------------------------------------------------
    { BOOT_NORMAL,            KERNEL_BOOT_THREAD_NAME(BOOT_NORMAL)          },
    { BOOT_CALIBRATION,       KERNEL_BOOT_THREAD_NAME(BOOT_CALIBRATION)     }
};

OsKernelEntryTask_t OsKernel::KernelTaskTable[] = {
//-----------------------------------------------------------------------------
//     Entry Task                           Task name
//-----------------------------------------------------------------------------
    { OsKernel::OsInitLogger,               F("Kernel: Init Logger")          },
    { OsKernel::OsInitBuses,                F("Kernel: Init Buses")           },
    { OsKernel::OsDumpSysInfo,              F("Kernel: Dump Kernel info")     },
    { OsKernel::OsInitBLE,                  F("Kernel: Init BLE")             },
    { OsKernel::OsInitSensors,              F("Kernel: Init Sensors")         },
    { OsKernel::OsInitTasks,                F("Kernel: Setup tasks")          },
    { OsKernel::NvStorageInit,              F("Kernel: Init NVROM"),          },
    { OsKernel::OsSetBootMode,              F("Kernek: Define Boot Mode")     }
};

const size_t OsKernel::BootModesCallTableSize = ARRAY_SIZE(OsKernel::BootModesCallTable);
const size_t OsKernel::KernelTaskTableSize = ARRAY_SIZE(OsKernel::KernelTaskTable);

BootModeEntry_f OsKernel::GetEntryPointForBootMode(SystemBootMode_t mode)
{
    size_t index;
    for (index = 0; index < OsKernel::BootModesCallTableSize; index++)
    {
        if (OsKernel::BootModesCallTable[index].mode == mode) {
            return OsKernel::BootModesCallTable[index].entryPoint;
        }
    }

    return NULL;
}
