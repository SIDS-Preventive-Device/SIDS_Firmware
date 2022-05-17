#include "system/utils/errors.h"
#include "system/modules/logger.h"
#include "system/utils/events.h"
#include "system/utils/array_size.h"
#include "kernel_internal.h"

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

#include "ble_uuids.h"

typedef struct {
    KernelBleServices_e Service;
    const char*         UUID;
    BLEDescriptor*      Description;
    BLECharacteristic*  Characteristic;
    const uint32_t      Props;
} BLECharacteristic_t;

typedef struct {
    const char* UUID;
    const char* Name;
    BLEService* pService;
} BLEService_t;

BLEService_t ServicesTable[] = {
    { POSITION_SERVICE_UUID,            "PositionService",          NULL },
    { TEMPERATURE_SERVICE_UUID,         "TemperatureService",       NULL },
    { BATTERY_SERVICE_UUID,             "BatteryService",           NULL },
    { DEVICE_INFO_SERVICE_UUID,         "DeviceInfoService",        NULL }
};

BLECharacteristic_t CharacteristicsTable[] = {
    { BLE_SERV_POSITION,            POSITION_CHARACTERISTIC_UUID,           new BLE2902(),      NULL, BLECharacteristic::PROPERTY_READ || BLECharacteristic::PROPERTY_NOTIFY        },
    { BLE_SERV_BATTERY,             BATTERY_LEVEL_CHARACTERISTIC_UUID,      new BLE2902(),      NULL, BLECharacteristic::PROPERTY_READ                                              },
    { BLE_SERV_TEMPERATURE,         TEMPERATURE_CHARACTERISTIC_UUID,        new BLE2902(),      NULL, BLECharacteristic::PROPERTY_READ                                              },
    { BLE_SERV_DEVICE_INFO,         FIRMWARE_VERSION_CHARACTERISTIC_UUID,   new BLE2902(),      NULL, BLECharacteristic::PROPERTY_READ                                              },
    { BLE_SERV_DEVICE_INFO,         STATUS_CODE_CHARACTERISTIC_UUID,        new BLE2902(),      NULL, BLECharacteristic::PROPERTY_READ                                              }
};

void DumpCharacteristicsTable();

bool OsKernel::OsInitBLE()
{
    if (configuration.bleDeviceName == nullptr) {
        configuration.bleDeviceName = F("CodeGravOs");
        logger << LOG_WARN << F("Bluetooth device not provided, using default: ") << configuration.bleDeviceName << EndLine;
    }

    BLEDevice::init(String(configuration.bleDeviceName).c_str());
    BLEServer* server = BLEDevice::createServer();

    const size_t servicesTableSize = ARRAY_SIZE(ServicesTable);
    const size_t characteristicsTableSize = ARRAY_SIZE(CharacteristicsTable);
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    BLECharacteristic_t* pCharactConfig = NULL;
    size_t index = 0;

    for (index = 0; index < servicesTableSize; index++) {
        ServicesTable[index].pService = server->createService(ServicesTable[index].UUID);
    }

    for (index = 0; index < characteristicsTableSize; index++) {
        pCharactConfig = &CharacteristicsTable[index];
        if (pCharactConfig->Service >= BLE_SERV_DELIM) {
            LogKernelError("Error initializing BLE Characteristic, specified service do not exists!");
            continue;
        }

        pCharactConfig->Characteristic = ServicesTable[pCharactConfig->Service].pService->createCharacteristic(
                pCharactConfig->UUID,
                pCharactConfig->Props
            );

        pCharactConfig->Characteristic->addDescriptor(pCharactConfig->Description);
    }

    for (index = 0; index < servicesTableSize; index++) {
        pAdvertising->addServiceUUID(ServicesTable[index].UUID);
        ServicesTable[index].pService->start();
    }

    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();

    String kernelVersion(KERNEL_VERSION);
    OsKernel::SetBLECharacteristicValue (BLE_CHT_FIRMWARE, (void*)kernelVersion.c_str(), kernelVersion.length() + 1);
    OsKernel::SetBLECharacteristicValue (BLE_CHT_STATUS, (uint16_t)0x0000);

    DumpCharacteristicsTable();

    return true;
}

void DumpCharacteristicsTable() {
    const size_t characteristicsTableSize = ARRAY_SIZE(CharacteristicsTable);
    BLECharacteristic_t* pCharactConfig = NULL;
    size_t index = 0;

    logger << LOG_INFO << F("BLE Characteristics Services Status Table") << EndLine;
    logger.printf("%20s | %40s | %13s\n", "Service", "UUID", "Descriptor");
    for (index = 0; index < characteristicsTableSize; index++) {
        pCharactConfig = &CharacteristicsTable[index];
        logger.printf("%20s | %40s | %13s\n", ServicesTable[pCharactConfig->Service].Name, pCharactConfig->UUID, pCharactConfig->Characteristic == NULL ? "--" : "OK");
    }
}

bool validateCharacteristicInit (KernelBleCharecteristics_e cht) {
    if (cht >= BLE_CHT_DELIM) {
        LogKernelError ("BLECharacteristic is unvalid!");
        return false;
    }
    if (CharacteristicsTable[cht].Characteristic == NULL) {
        LogKernelError ("BLE Characteristic is uninitialized!");
        return false;
    }
    return true;
}

void OsKernel::SetBLECharacteristicValue (KernelBleCharecteristics_e cht, void* data, size_t len) {
    if (validateCharacteristicInit(cht)) {
        return;
    }

    if (data == NULL || len == 0) {
        LogKernelError ("set Data is unvalid");
        return;
    }

    CharacteristicsTable[cht].Characteristic->setValue((uint8_t*)data, len);
    if (CharacteristicsTable[cht].Props & BLECharacteristic::PROPERTY_NOTIFY) {
        CharacteristicsTable[cht].Characteristic->notify();
    }
}

void OsKernel::SetBLECharacteristicValue (KernelBleCharecteristics_e cht, String parameter)
{
    if (validateCharacteristicInit(cht)) {
        return;
    }

    CharacteristicsTable[cht].Characteristic->setValue(parameter.c_str());
    if (CharacteristicsTable[cht].Props & BLECharacteristic::PROPERTY_NOTIFY) {
        CharacteristicsTable[cht].Characteristic->notify();
    }
}

void OsKernel::SetBLECharacteristicValue (KernelBleCharecteristics_e cht, uint16_t parameter)
{
    if (validateCharacteristicInit(cht)) {
        return;
    }

    CharacteristicsTable[cht].Characteristic->setValue(parameter);
    if (CharacteristicsTable[cht].Props & BLECharacteristic::PROPERTY_NOTIFY) {
        CharacteristicsTable[cht].Characteristic->notify();
    }
}

void OsKernel::SetBLECharacteristicValue (KernelBleCharecteristics_e cht, uint32_t parameter)
{
    if (validateCharacteristicInit(cht)) {
        return;
    }

    CharacteristicsTable[cht].Characteristic->setValue(parameter);
    if (CharacteristicsTable[cht].Props & BLECharacteristic::PROPERTY_NOTIFY) {
        CharacteristicsTable[cht].Characteristic->notify();
    }
}

void OsKernel::SetBLECharacteristicValue (KernelBleCharecteristics_e cht, int32_t parameter)
{
    if (validateCharacteristicInit(cht)) {
        return;
    }

    CharacteristicsTable[cht].Characteristic->setValue(parameter);
    if (CharacteristicsTable[cht].Props & BLECharacteristic::PROPERTY_NOTIFY) {
        CharacteristicsTable[cht].Characteristic->notify();
    }
}
