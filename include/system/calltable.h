#if !defined(__CODE_GRAV_SYSTEM_CALL_TABLES_H__)
#define __CODE_GRAV_SYSTEM_CALL_TABLES_H__

#include "bootmode.h"

typedef void (*BootModeEntry_f)(SystemCore *pKernel);

typedef struct {
    SystemBootMode_t mode;
    BootModeEntry_f  entryPoint;
} BootModesCallTableEntry_t;

extern BootModesCallTableEntry_t BootModesCallTable[];
extern const size_t BootModesCallTableSize;

extern BootModeEntry_f GetEntryPointForBootMode(SystemBootMode_t);

#endif // __CODE_GRAV_SYSTEM_CALL_TABLES_H__
