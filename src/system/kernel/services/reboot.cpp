#include "../kernel_internal.h"
#include "system/utils/errors.h"
#include "system/modules/logger.h"

KERNEL_SERVICE(OS_SERVICE_REBOOT) {
    ESP.restart();
}
