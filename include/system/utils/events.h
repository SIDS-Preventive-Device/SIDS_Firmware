#if !defined(__CODE_GRAV_SYSTEM_UTILS_KEY_EVENTS_H__)
#define __CODE_GRAV_SYSTEM_UTILS_KEY_EVENTS_H__

#include <Print.h>

typedef bool (*WaitCallback_t)(void *pContext);

extern bool wait(WaitCallback_t pollFunction, void *pContext = NULL, uint32_t timeout_ms = 1000UL, Print *cout = NULL, char waitChar = '.');

#endif // __CODE_GRAV_SYSTEM_UTILS_KEY_EVENTS_H__
