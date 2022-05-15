#include "system/kernel/nvstorage.h"
#include "system/kernel/core.h"
#include "kernel_internal.h"

#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"
#include <stdlib.h>
#include <string.h>

const char *NvStorageNameKey = "NVcodeGrav";
static nvs_handle storage_h;
static bool NvStorageIsInit = false;

template<>
bool __NvStorageInternalRead<uint8_t>(const char* key, uint8_t* data) {
    return nvs_get_u8(storage_h, key, data) == ESP_OK;
}

template<>
bool __NvStorageInternalRead<uint16_t>(const char* key, uint16_t* data) {
    return nvs_get_u16(storage_h, key, data) == ESP_OK;
}

template<>
bool __NvStorageInternalRead<uint32_t>(const char* key, uint32_t* data) {
    return nvs_get_u32(storage_h, key, data) == ESP_OK;
}

template<>
bool __NvStorageInternalRead<int8_t>(const char* key, int8_t* data) {
    return nvs_get_i8(storage_h, key, data) == ESP_OK;
}

template<>
bool __NvStorageInternalRead<int16_t>(const char* key, int16_t* data) {
    return nvs_get_i16(storage_h, key, data) == ESP_OK;
}

template<>
bool __NvStorageInternalRead<int32_t>(const char* key, int32_t* data) {
    return nvs_get_i32(storage_h, key, data) == ESP_OK;
}

template<>
bool __NvStorageInternalRead<char*>(const char* key, char** data) {
    size_t size = 0;
    esp_err_t err = nvs_get_str(storage_h, key, NULL, &size);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND)
    {
        return false;
    }

    logger << LOG_DEBUG << F("NVStorage: Variable ") << key;
    if (size == 0) {
        logger << LOGGER_TEXT_RED << F(" not found") << EndLine;
        return false;
    }

    char *pDataBuffer = (char*)malloc(size);
    memset(pDataBuffer, 0x00, size);

    err = nvs_get_str(storage_h, key, pDataBuffer, &size);
    if (err != ESP_OK)
    {
        free (pDataBuffer);
        logger << LOGGER_TEXT_RED << F(" error on load") << EndLine;
        return false;
    }

    *data = pDataBuffer;

    logger << LOGGER_TEXT_GREEN << F(" loaded") << EndLine;
    return true;
}


template<>
bool __NvStorageInternalRead<Blob_t>(const char* key, Blob_t* data) {
    size_t size = 0;
    esp_err_t err = nvs_get_blob(storage_h, key, NULL, &size);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND)
    {
        return false;
    }

    logger << LOG_DEBUG << F("NVStorage: Variable ") << key;
    if (size == 0) {
        logger << LOGGER_TEXT_RED << F(" not found") << EndLine;
        return false;
    }

    uint8_t *pDataBuffer = (uint8_t*)malloc(size);
    memset(pDataBuffer, 0x00, size);

    err = nvs_get_blob(storage_h, key, pDataBuffer, &size);
    if (err != ESP_OK)
    {
        free (pDataBuffer);
        logger << LOGGER_TEXT_RED << F(" error on load") << EndLine;
        return false;
    }

    data->pData = pDataBuffer;
    data->size = size;

    logger << LOGGER_TEXT_GREEN << F(" loaded") << EndLine;
    return true;
}

template<>
bool __NvStorageInternalWrite<uint8_t>(const char* key, uint8_t data) {
    return nvs_set_u8(storage_h, key, data) == ESP_OK;
}

template<>
bool __NvStorageInternalWrite<uint16_t>(const char* key, uint16_t data) {
    return nvs_set_u16(storage_h, key, data) == ESP_OK;
}

template<>
bool __NvStorageInternalWrite<uint32_t>(const char* key, uint32_t data) {
    return nvs_set_u32(storage_h, key, data) == ESP_OK;
}

template<>
bool __NvStorageInternalWrite<int8_t>(const char* key, int8_t data) {
    return nvs_set_i8(storage_h, key, data) == ESP_OK;
}

template<>
bool __NvStorageInternalWrite<int16_t>(const char* key, int16_t data) {
    return nvs_set_i16(storage_h, key, data) == ESP_OK;
}

template<>
bool __NvStorageInternalWrite<int32_t>(const char* key, int32_t data) {
    return nvs_set_i32(storage_h, key, data) == ESP_OK;
}

template<>
bool __NvStorageInternalWrite<char*>(const char* key, char* data) {
    return nvs_set_str(storage_h, key, data) == ESP_OK;
}

template<>
bool __NvStorageInternalWrite<Blob_t>(const char* key, Blob_t data) {
    return nvs_set_blob(storage_h, key, data.pData, data.size) == ESP_OK;
}

bool OsKernel::NvStorageInit()
{
    esp_err_t err = nvs_flash_init();
    NvStorageIsInit = false;

    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        err = nvs_flash_erase();
        if (err != ESP_OK) {
            LogKernelError("Error on non-volatile memory initialization!");
            return false;
        }
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        LogKernelError("Error on non-volatile memory initialization!");
        return false;
    }

    err = nvs_open(NvStorageNameKey, NVS_READWRITE, &storage_h);
    if (err != ESP_OK) {
        LogKernelError("Error trying to opening storage!");
        return false;
    }

    NvStorageIsInit = true;
    return true;
}

bool OsKernel::NvStorageIsOk() 
{
    return NvStorageIsInit;
}

bool OsKernel::__NsStorageSaveChanges() {
    return nvs_commit(storage_h) == ESP_OK;
}
