#include <Wire.h>
#include "common2.h"
#include "SoftI2CMaster.h"
#include "I2CDevice.h"
#include "ADXLAccelerometer.h"
#include "HMCCompass.h"
#include "L3GGyroscope.h"

//Digital pins, not analog pins
#define SCL 2
#define SDA1 3

static Gyroscope gyro1;
static Gyroscope gyro2;

void setup(){
  Serial.begin(230400);
  gyro1 = Gyroscope(SDA1 + 3, SCL);
  gyro1.init();
  gyro2 = Gyroscope(SDA1 + 2, SCL);
  gyro2.init();
}

void loop(){
  gyro1.update(); //Can never be read
  floatVec3 angle1 = gyro1.getRaw();
  gyro2.update(); //Can be read if gyro1 disabled
  floatVec3 angle2 = gyro2.getRaw();
  sPrintF(angle1.x, angle1.y, angle1.z, false);
  sPrintF(angle2.x, angle2.y, angle2.z, true);
}
