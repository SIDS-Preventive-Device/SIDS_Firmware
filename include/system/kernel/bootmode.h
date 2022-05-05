#if !defined(__CODE_GRAV_INCLUDE_SYSTEM_BOOT_MODE_H__)
#define __CODE_GRAV_INCLUDE_SYSTEM_BOOT_MODE_H__

#include <WString.h>

typedef enum {
    BOOT_NORMAL,
    BOOT_CALIBRATION,
    BOOT_DELIMITER
} SystemBootMode_t;

#define KERNEL_BOOT_THREAD_NAME(module_name) __system_##module_name
#define KERNEL_BOOT_THREAD_FUNC(module_name) void KERNEL_BOOT_THREAD_NAME(module_name)(void)

extern KERNEL_BOOT_THREAD_FUNC(BOOT_NORMAL);
extern KERNEL_BOOT_THREAD_FUNC(BOOT_CALIBRATION);

namespace OsKernel {

extern const __FlashStringHelper *BootModeStrMap[];
extern const __FlashStringHelper *GetBootModeStr(SystemBootMode_t);

}; // namespace OsKernel

#endif // __CODE_GRAV_INCLUDE_SYSTEM_BOOT_MODE_H__
