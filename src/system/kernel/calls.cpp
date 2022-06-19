#include "system/kernel/calls.h"
#include "system/utils/errors.h"
#include "system/utils/array_size.h"
#include "system/modules/logger.h"
#include "kernel_internal.h"

KernelServicesEntry_t OsKernel::KernelServicesCallTable[] = {
//-----------------------------------------------------------------------------------------------------------------------------------------------
//    Service                                   Entry Point                                                         Name
//-----------------------------------------------------------------------------------------------------------------------------------------------
    { OS_SERVICE_UPDATE_ORIENTATION,            KERNEL_SERVICE_NAME(OS_SERVICE_UPDATE_ORIENTATION),                 F("UpdateOrientation")      },
    { OS_SERVICE_UPDATE_TEMPERATURE,            KERNEL_SERVICE_NAME(OS_SERVICE_UPDATE_TEMPERATURE),                 F("UpdateTemperature")      },
    { OS_SERVICE_THROW_POSITION_RISK_ALERT,     KERNEL_SERVICE_NAME(OS_SERVICE_THROW_POSITION_RISK_ALERT),          F("AlertPositionRisk")      },
    { OS_SERVICE_THROW_BREATH_RISK_ALERT,       KERNEL_SERVICE_NAME(OS_SERVICE_THROW_BREATH_RISK_ALERT),            F("AlertBreathRisk")        },
    { OS_SERVICE_REBOOT,                        KERNEL_SERVICE_NAME(OS_SERVICE_REBOOT),                             F("Reboot")                 }
};

const size_t OsKernel::KernelServicesCallTableSize = ARRAY_SIZE(OsKernel::KernelServicesCallTable);

void OsKernel::OsCall(KernelService_e service, void *pContext)
{
    if (service >= OsKernel::KernelServicesCallTableSize) {
        LogKernelError("Try to call a kernel service that do not exist!!!");
        return ;
    }

    // logger << LOG_DEBUG << F("@Kernel: Call ") << OsKernel::KernelServicesCallTable[service].name << EndLine;
    OsKernel::KernelServicesCallTable[service].task(pContext);
}
