#include "system/kernel/core.h"
#include "system/utils/errors.h"
#include "system/modules/logger.h"
#include "system/utils/i2c.h"
#include "system/utils/events.h"
#include "system/kernel/entrytable.h"
#include "kernel_internal.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <Arduino.h>
#include <Wire.h>

KernelLogger logger (nullptr);

SystemConfig_t      OsKernel::configuration;
SystemBootMode_t    OsKernel::bootMode;
TaskHandle_t        OsKernel::systemMainTask_h;
esp_timer_handle_t  OsKernel::rfshSensorState_h;
esp_timer_handle_t  OsKernel::readBatteryTimer_h;

const esp_timer_create_args_t refreshSensorState = {
    .callback = KERNEL_CALLBACK_NAME(UPDATE_SERVICES_REFRESH_SENSOR_STATE),
    .arg = NULL,
    .dispatch_method = ESP_TIMER_TASK,
    .name = "RefreshSensorsState"
};

const esp_timer_create_args_t readBatteryTimer = {
    .callback = KERNEL_CALLBACK_NAME(UPDATE_SERVICES_BATTERY),
    .arg = NULL,
    .dispatch_method = ESP_TIMER_TASK,
    .name = "ReadBattery"
};

typedef struct {
    const char* name;
    ISensor *sensor;
    bool required;
} SensorsEntryTable_t;

using OsKernel::configuration;
using OsKernel::bootMode;
using OsKernel::systemMainTask_h;
using OsKernel::BootModesCallTable;
using OsKernel::BootModesCallTableSize;
using OsKernel::KernelTaskTable;
using OsKernel::KernelTaskTableSize;
using OsKernel::rfshSensorState_h;

SystemBootMode_t OsKernel::OsGetBootMode()
{
    return bootMode;
}

void OsKernel::OsInit(SystemConfig_t config)
{
    size_t index = 0;
    OsKernel::configuration = config;
    OsKernel::bootMode      = BOOT_NORMAL;

    for (index = 0; index < KernelTaskTableSize; index ++) {
        logger << LOG_MASTER << LOGGER_TEXT_YELLOW << KernelTaskTable[index].name << EndLine;
        if (!KernelTaskTable[index].task()) {
            LogKernelError ("Task return false, panic error!");
            while (true) { }
        }
    }

    if (rfshSensorState_h == NULL) {
        logger << LOG_ERROR << F("Refresh sensors is disabled!") << EndLine;
    } else {
        esp_timer_start_periodic (rfshSensorState_h, 60000000UL);
    }

    if (readBatteryTimer_h == NULL) {
        logger << LOG_ERROR << F("Read battery is disabled!") << EndLine;
    } else {
        esp_timer_start_periodic (readBatteryTimer_h, 10000000UL);
    }

    logger << LOG_MASTER << F("Initialization done") << EndLine;

    xTaskNotify (systemMainTask_h, 0x00, eNoAction);
    vTaskDelete(NULL);
}

bool OsKernel::OsSetBootMode()
{
    logger << LOG_MASTER << F("Press a key to boot as calibration...") << EndLine;

    bool bootAsCalibration = wait([](void *pSerialPort) -> bool {
        return ((HardwareSerial*)pSerialPort)->available() > 0;
    }, configuration.serialPort, 3000UL, &logger, '.');

    if (bootAsCalibration) {
        bootMode = BOOT_CALIBRATION;
    }

    logger << LOG_INFO << F("Booting as ") << GetBootModeStr(bootMode) << EndLine;
    return true;
}

bool OsKernel::OsInitBuses()
{
    if (configuration.serialPort != nullptr) {
        configuration.serialPort->begin(
            configuration.baudrate
            );
    } else {
        LogKernelError ("Serial Bus instance is NULL!!!");
        return false;
    }

    if (configuration.i2cBus != nullptr) {
        configuration.i2cBus->begin(-1, -1, 400000L);
    } else {
        LogKernelError ("I2C Bus instance is NULL!!!");
        return false;
    }
    return true;
}

bool OsKernel::OsInitLogger()
{
    logger.setLogOutput(configuration.serialPort);
    logger.setLogLevel(configuration.logLevel);
    return true;
}

bool OsKernel::OsInitSensors()
{
    static SensorsEntryTable_t sensorsTable[] = {
        { "OrientationSensor", configuration.orientationSensor, true },
        { "TemperatureSensor", configuration.temperatureSensor, false },
        { "BatterySensor", configuration.batterySensor, false }
    };
    const uint8_t sensorsTableSize = ARRAY_SIZE(sensorsTable);
    uint8_t index = 0;

    for (index = 0; index < sensorsTableSize; index++) {
        //
        // Current sensor interface is undefined?
        //
        if (sensorsTable[index].sensor == nullptr) {
            //
            // If required throw a kernel error, otherwise only show a warning
            //
            if (sensorsTable[index].required) {
                LogKernelError ("Required sensor interface not provided!");
                return false;
            } else {
                logger << LOG_WARN << F("Sensor interface not provided: ") << sensorsTable[index].name << EndLine;
                continue;
            }
        }

        //
        // Error on initization?
        //
        sensorsTable[index].sensor->setName(sensorsTable[index].name);
        if (sensorsTable[index].sensor->init() == false) {
            //
            // If required throw a kernel error, otherwise only show a warning
            //
            if (sensorsTable[index].required) {
                LogKernelError ("Required sensor interface error on initialization!");
                return false;
            } else {
                logger << LOG_WARN << F("Sensor inteface error on init: ") << sensorsTable[index].name << EndLine;
            }
        }
    }

    if (configuration.riskAlert == nullptr) {
        logger << LOG_WARN << F("Risk alert provider is not defined, disabling alerts!") << EndLine;
    } else {
        configuration.riskAlert->Configure();
        configuration.riskAlert->Alert(1000);
    }

    if (configuration.batterySensor && esp_timer_create(&readBatteryTimer, &readBatteryTimer_h) != ESP_OK) {
        LogKernelError ("Fail to initialize read battery task");
        return false;
    }

    if (esp_timer_create(&refreshSensorState, &rfshSensorState_h) != ESP_OK) {
        LogKernelError ("Fail to initialize refresh sensor state task");
        return false;
    }

    return true;
}

bool OsKernel::OsInitTasks()
{
    xTaskCreatePinnedToCore (
        [](void *__) -> void {
            xTaskNotifyWait (0, 0, NULL, portMAX_DELAY);

            BootModeEntry_f entryPoint = OsKernel::GetEntryPointForBootMode(OsKernel::OsGetBootMode());

            if (!entryPoint) {
                LogKernelError("Not found entry point for current boot mode!!!");
                assert(false);
                vTaskDelete(NULL);
            }

            entryPoint ();

            LogKernelError("System error, kernel out-phase code reached, this is Fatal!");

            assert(false);
            vTaskDelete(NULL);
        },
        "CodeGrav_Kernel", 0x4000, NULL, 1, &systemMainTask_h, 1
    );

    return true;
}

bool OsKernel::OsDumpSysInfo()
{
    logger << LOG_MASTER << EndLine << LOGGER_TEXT_BOLD << LOGGER_TEXT_YELLOW <<
    F("--------------------------------------------------------\n") <<
    F("   _____             _        _____                     \n") <<
    F("  /  __ \\           | |      |  __ \\                    \n") <<
    F("  | /  \\/  ___    __| |  ___ | |  \\/ _ __   __ _ __   __\n") <<
    F("  | |     / _ \\  / _` | / _ \\| | __ | '__| / _` |\\ \\ / /\n") <<
    F("  | \\__/\\| (_) || (_| ||  __/| |_\\ \\| |   | (_| | \\ V / \n") <<
    F("   \\____/ \\___/  \\__,_| \\___| \\____/|_|    \\__,_|  \\_/  \n") <<
    F("--------------------------------------------------------\n") <<
    LOG_MASTER << F("Fimware version: ") << KERNEL_VERSION << EndLine <<
    LOG_MASTER << F("Build date: ") << __DATE__ << F(" ") << __TIME__ << EndLine <<
    LOG_MASTER << F("Chip revision: ") << ESP.getChipRevision() << EndLine <<
    LOG_MASTER << F("Clock Speed: ") << ESP.getCpuFreqMHz() << F("mHz") << EndLine;

    DumpI2Cdevices(*configuration.i2cBus);

    return true;
}

SystemConfig_t* OsKernel::OsGetSysConfigPtr () {
    return &OsKernel::configuration;
}
