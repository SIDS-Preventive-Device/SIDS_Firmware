#if !defined(__CODE_GRAV_SYSTEM_KERNEL_ENTRY_TABLE_H__)
#define __CODE_GRAV_SYSTEM_KERNEL_ENTRY_TABLE_H__

#include "bootmode.h"
#include <WString.h>

typedef void (*BootModeEntry_f)(void);
typedef bool (*OsKernelEntryTask)(void);

typedef struct {
    SystemBootMode_t mode;
    BootModeEntry_f  entryPoint;
} BootModesCallTableEntry_t;

typedef struct {
    OsKernelEntryTask          task;
    const __FlashStringHelper* name;
} OsKernelEntryTask_t;

namespace OsKernel {

extern BootModesCallTableEntry_t BootModesCallTable[];
extern const size_t BootModesCallTableSize;

extern OsKernelEntryTask_t KernelTaskTable[];
extern const size_t KernelTaskTableSize;

extern BootModeEntry_f GetEntryPointForBootMode(SystemBootMode_t);

} // namespace OsKernel

#endif // __CODE_GRAV_SYSTEM_KERNEL_ENTRY_TABLE_H__
