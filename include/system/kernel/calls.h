#if !defined(__CODE_GRAV_SYSTEM_KERNEL_CALLS_H__)
#define __CODE_GRAV_SYSTEM_KERNEL_CALLS_H__

#include <WString.h>
#include <system/utils/types.h>

//
// TODO: Under development
//

typedef enum {
    OS_SERVICE_UPDATE_ORIENTATION,
    OS_SERVICE_UPDATE_TEMPERATURE,
    OS_SERVICE_THROW_POSITION_RISK_ALERT,
    OS_SERVICE_THROW_BREATH_RISK_ALERT,
    OS_SERVICE_REBOOT,
    OS_SERVICE_DELIMITER
} KernelService_e;

typedef void (*KernelServicesEntry_f)(void *pContext);

typedef struct {
    KernelService_e            service;
    KernelServicesEntry_f      task;
    const __FlashStringHelper* name;
} KernelServicesEntry_t;

namespace OsKernel
{

extern KernelServicesEntry_t KernelServicesCallTable[];
extern const size_t KernelServicesCallTableSize;

extern void OsCall(KernelService_e service, void *pContext);

} // namespace OsKernel


#endif // __CODE_GRAV_SYSTEM_KERNEL_CALLS_H__
