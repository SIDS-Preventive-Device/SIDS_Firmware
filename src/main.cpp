#include <Arduino.h>
#include <Wire.h>

#include <system/core.h>
#include <system/builder.h>
#include <system/errors.h>
#include <system/drivers/mpu9250.h>

void setup() {
  //
  // Configure, initalize and execute system firmware!
  //
  SystemBuilder()
    .setSerialConfig(Serial, 115200)
    .setI2Cport(Wire)
    .setLogLevel(LOG_DEBUG)
    .addSensor<SensorMPU9250>()
    .build()->init().execute();

  //
  // Should not return
  //
  LogKernelError ("Unexpected code reached!");
}

void loop() {}