#include <Arduino.h>
#include <Wire.h>

#include <system/kernel/builder.h>
#include <system/utils/errors.h>
#include <system/drivers/mpu9250.h>

void setup() {
  //
  // Configure, initalize and execute system firmware!
  //
  SystemConfig_t config = SystemBuilder()
    .setSerialConfig(Serial, 115200)
    .setI2Cport(Wire)
    .setLogLevel(LOG_DEBUG)
    .setOrientationSensor<SensorMPU9250>()
    .build();

  OsKernel::OsInit(config);

  //
  // Should not return
  //
  LogKernelError ("Unexpected code reached!");
}

void loop() {}