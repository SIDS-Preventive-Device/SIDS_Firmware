#include "../kernel_internal.h"
#include "system/utils/errors.h"
#include "system/modules/logger.h"

KERNEL_CALLBACK(UPDATE_SERVICES_CB) {
    logger << LOG_INFO << F("Refresh sensor connections") << EndLine;
}

KERNEL_SERVICE(OS_SERVICE_UPDATE_ORIENTATION) {

    SystemConfig_t *config = OsKernel::OsGetSysConfigPtr();
    OrientationData_t *pData = (OrientationData_t*)pContext;
    bool success = false;

    if (pData == nullptr) {
        logger << LOG_WARN << F("Can't update any sensors, no output buffer specified!") << EndLine;
        return;
    }

    success = config->orientationSensor->update();
    if (!success) {
        LogKernelError("Error on updating orientation sensor!");
        return;
    }

    pData->acceleration  = config->orientationSensor->getAccVector();
    pData->rotation      = config->orientationSensor->getGiroVector();
    pData->magnetometer  = config->orientationSensor->getMagVector();
}
