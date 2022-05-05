#if !defined(__CODE_GRAV_SYSTEM_KERNEL_CALLS_H__)
#define __CODE_GRAV_SYSTEM_KERNEL_CALLS_H__

#include <WString.h>

//
// TODO: Under development
//

namespace OsKernel
{

typedef enum {
    OS_CALL_UPDATE_SENSORS,
    OS_CALL_DELIMITER
} KernelService_e;

typedef void (*KernelServicesEntry_f)(void);

typedef struct {
    KernelServicesEntry_f      task;
    const __FlashStringHelper* name;
} KernelServicesEntry_t;

extern KernelServicesEntry_t KernelServicesCallTable[];
extern const size_t KernelServicesCallTableSize;

} // namespace OsKernel


#endif // __CODE_GRAV_SYSTEM_KERNEL_CALLS_H__
