#include "system/errors.h"
#include "system/logger.h"
#include "system/utils/array_size.h"

const __FlashStringHelper *ErrorTypeStrMap[] = {
    F("Error on Kernel Initialization ocurred! at "),
    F("Critical Error at "),
    F("Minor error at ")
};

void __LogError(const char* file, int line, ErrorType_e error_type, const char* msg)
{
    if (error_type >= ARRAY_SIZE(ErrorTypeStrMap)) {
        error_type = ERROR_CRITICAL;
    }
    logger << LOG_ERROR
        << LOGGER_TEXT_RED << ErrorTypeStrMap[(uint8_t)error_type] << file << F(":") << line << EndLine
        << LOG_ERROR << msg << EndLine;
#if STOP_AT_CRITICAL
    while (error_type == ERROR_CRITICAL) {
        ;
    }
#endif
}

