#include <Arduino.h>
#include <I2Cdev.h>

#include <system/kernel/builder.h>
#include <system/utils/errors.h>
#include <system/drivers/battery.h>
#include <system/drivers/buzzer.h>
#include <system/drivers/mpu9250.h>

void setup() {
  //
  // Configure, initalize and execute system firmware!
  //
  SystemConfig_t config = SystemBuilder()
    .setSerialConfig(Serial, 115200)
    .setI2Cport(i2c0)
    .setLogLevel(LOG_DEBUG)
    .setBatterySensor<BatterySensor>()
    .setOrientationSensor<SensorMPU9250>()
    .setRiskAlert<Buzzer>()
    .enableBLE(F("GravDevice"))
    .build();

  OsKernel::OsInit(config);

  //
  // Should not return
  //
  LogKernelError ("Unexpected code reached!");
}

void loop() {}