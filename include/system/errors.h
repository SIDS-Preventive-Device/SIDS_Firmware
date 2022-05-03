#if !defined(__CODE_GRAV_SYSTEM_ERRORS_H__)
#define __CODE_GRAV_SYSTEM_ERRORS_H__

typedef enum {
    ERROR_KERNEL,
    ERROR_CRITICAL,
    ERROR_MINOR,
    ERROR_DELIMITER
} ErrorType_e;

#define STOP_AT_CRITICAL 0

extern void __LogError(const char* file, int line, ErrorType_e error_type, const char* msg);

#define LogKernelError(MSG) __LogError(__FILE__, __LINE__, ERROR_KERNEL, MSG)

#define LogMinorError(MSG) __LogError(__FILE__, __LINE__, ERROR_MINOR, MSG)

#define LogCriticalError(MSG) __LogError(__FILE__, __LINE__, ERROR_CRITICAL, MSG)

#endif // __CODE_GRAV_SYSTEM_ERRORS_H__
