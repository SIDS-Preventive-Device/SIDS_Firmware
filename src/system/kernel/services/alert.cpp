#include "../kernel_internal.h"
#include "system/utils/errors.h"
#include "system/modules/logger.h"

KERNEL_SERVICE(OS_SERVICE_THROW_POSITION_RISK_ALERT) {
    SystemConfig_t *config = OsKernel::OsGetSysConfigPtr();
    uint8_t *pTimeMS = (uint8_t*)pContext;
    uint8_t alertTime = 0;

    if (pTimeMS != nullptr) {
        alertTime = *pTimeMS;
    }

    config->riskAlert->Alert(alertTime);
}
