#if !defined(__CODE_GRAV_SYSTEM_KERNEL_NVSTORAGE_H__)
#define __CODE_GRAV_SYSTEM_KERNEL_NVSTORAGE_H__

#include <stdint.h>
#include "system/modules/logger.h"
#include "system/utils/errors.h"

typedef struct
{
    uint16_t size;
    void *pData;
} Blob_t;

template <typename T>
bool __NvStorageInternalRead(const char* key, T* data);

template <typename T>
bool __NvStorageInternalWrite(const char* key, T data);

namespace OsKernel
{
    extern bool NvStorageIsOk();

    extern bool __NsStorageSaveChanges();

    template <typename T>
    bool NvVariableGet(const char *varKey, T *data)
    {
        if (!NvStorageIsOk())
        {
            logger << LOG_WARN << F("non-volatile memory module is disabled due to initialization failed!") << EndLine;
            return false;
        }

        if (data == NULL)
        {
            LogMinorError("Output pointer is NULL!");
            return false;
        }

        if (!__NvStorageInternalRead<T>(varKey, data))
        {
            LogKernelError("Error reading non-volatile variable");
            return false;
        }

        return true;
    }

    template <typename T>
    bool NvVariableSet(const char *varKey, T data)
    {
        if (!NvStorageIsOk())
        {
            logger << LOG_WARN << F("non-volatile memory module is disabled due to initialization failed!") << EndLine;
            return false;
        }

        if (!__NvStorageInternalWrite<T>(varKey, data))
        {
            LogKernelError("Error writing non-volatile variable");
            return false;
        }

        __NsStorageSaveChanges();
        return true;
    }

} // namespace OsKernel

#endif // __CODE_GRAV_SYSTEM_KERNEL_NVSTORAGE_H__
