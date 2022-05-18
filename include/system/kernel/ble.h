#if !defined(__CODE_GRAV_SYSTEM_KERNEL_BLE_H__)
#define __CODE_GRAV_SYSTEM_KERNEL_BLE_H__

typedef enum {
    BLE_SERV_POSITION,
    BLE_SERV_TEMPERATURE,
    BLE_SERV_BATTERY,
    BLE_SERV_DEVICE_INFO,
    BLE_SERV_DELIM
} KernelBleServices_e;

typedef enum {
    BLE_CHT_POSITION,
    BLE_CHT_BATTERY,
    BLE_CHT_TEMPERATURE,
    BLE_CHT_FIRMWARE,
    BLE_CHT_STATUS,
    BLE_CHT_DELIM
} KernelBleCharecteristics_e;

namespace OsKernel {

extern void SetBLECharacteristicValue (KernelBleCharecteristics_e cht, void* data, size_t len);
extern void SetBLECharacteristicValue (KernelBleCharecteristics_e cht, String parameter);
extern void SetBLECharacteristicValue (KernelBleCharecteristics_e cht, uint16_t parameter);
extern void SetBLECharacteristicValue (KernelBleCharecteristics_e cht, uint32_t parameter);
extern void SetBLECharacteristicValue (KernelBleCharecteristics_e cht, int32_t parameter);

} // namespace OsKernel
#endif // __CODE_GRAV_SYSTEM_KERNEL_BLE_H__
